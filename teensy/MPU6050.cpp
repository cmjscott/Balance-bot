#include <Wire.h>

#include "MPU6050.h"


MPU6050Class MPU6050;

////////////////////////////////////////////////////////////////////////////////////////////////////

void MPU6050Class::init()
{
	Wire.begin();

	write(PWR_MGMT_1, 0); // Wake up
	write(ACCEL_CONFIG, 0 << 3); // Set accelerometers measuring range from -2g to 2g
	write(GYRO_CONFIG, 3 << 3); // Set gyroscopes measuring range from -2000°/s to 2000°/s 

	m_attitude = {1, 0, 0, 0};
	m_last_update = micros();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MPU6050Class::update()
{
	// Get raw data
	const unsigned long now = micros();
	get_raw_values();

	// Compute elapsed time
	const float dt = (now - m_last_update) / 1e6;
	m_last_update = now;

	// Compute quaternions
	const Quaternion accel_axis =
	{
		0,
		((ACCEL_X_PLUS_G + ACCEL_X_MINUS_G) / 2 - m_raw_accel_x) / ((ACCEL_X_PLUS_G - ACCEL_X_MINUS_G) / 2),
		((ACCEL_Y_PLUS_G + ACCEL_Y_MINUS_G) / 2 - m_raw_accel_y) / ((ACCEL_Y_PLUS_G - ACCEL_Y_MINUS_G) / 2),
		((ACCEL_Z_PLUS_G + ACCEL_Z_MINUS_G) / 2 - m_raw_accel_z) / ((ACCEL_Z_PLUS_G - ACCEL_Z_MINUS_G) / 2)
	};
	const Quaternion half_gyro_axis =
	{
		0,
		(m_raw_gyro_x - GYRO_X_ZERO) * dt * 0.5 * radians(2000) / 32768,
		(m_raw_gyro_y - GYRO_Y_ZERO) * dt * 0.5 * radians(2000) / 32768,
		(m_raw_gyro_z - GYRO_Z_ZERO) * dt * 0.5 * radians(2000) / 32768
	};

	// Compute the estimated attitude using the gyroscopes only
	const Quaternion gyro_attitude = (m_attitude + m_attitude * half_gyro_axis).normalized();

	// Correct the estimated attitude using the accelerometers
	const Quaternion local_gravity = gyro_attitude.conjugate() * Quaternion({0, 0, 0, -1}) * gyro_attitude;
	const Quaternion correction = (Quaternion({1, 0, 0, 0}) - accel_axis.normalized() * local_gravity).conjugate().normalized();
	const Quaternion accel_attitude = gyro_attitude * correction;

	// Merge estimated attitudes using a complementary filter
	m_attitude = lerp(accel_attitude, gyro_attitude, FILTER_PARAM);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MPU6050Class::write(uint8_t address, uint8_t value) const
{
	Wire.beginTransmission(MPU6050_ADDRESS);
	Wire.write(address);
	Wire.write(value);
	Wire.endTransmission(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t MPU6050Class::read(uint8_t address) const
{
	Wire.beginTransmission(MPU6050_ADDRESS);
	Wire.write(address);
	Wire.endTransmission(false);
	Wire.requestFrom(MPU6050_ADDRESS, uint8_t(1), uint8_t(true));
	return Wire.read();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MPU6050Class::get_raw_values()
{
	Wire.beginTransmission(MPU6050_ADDRESS);
	Wire.write(ACCEL_XOUT_H); // starting with register 0x3B (ACCEL_XOUT_H)
	Wire.endTransmission(false);
	Wire.requestFrom(MPU6050_ADDRESS, uint8_t(14), uint8_t(true)); // request a total of 14 registers
	m_raw_accel_x = Wire.read() << 8 | Wire.read();
	m_raw_accel_y = Wire.read() << 8 | Wire.read();
	m_raw_accel_z = Wire.read() << 8 | Wire.read();
	m_raw_temp    = Wire.read() << 8 | Wire.read();
	m_raw_gyro_x  = Wire.read() << 8 | Wire.read();
	m_raw_gyro_y  = Wire.read() << 8 | Wire.read();
	m_raw_gyro_z  = Wire.read() << 8 | Wire.read();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
