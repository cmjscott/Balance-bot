#include <Wire.h>

#include "Adafruit9DOF.h"


Adafruit9DOFClass Adafruit9DOF;

////////////////////////////////////////////////////////////////////////////////////////////////////

void Adafruit9DOFClass::init()
{
	Wire.begin();

	write(LSM303_ACCEL_ADDRESS, LSM303_ACCEL_CTRL_REG1_A, 0x57); // Wake up
	write(L3GD20_ADDRESS, L3GD20_CTRL_REG1, 0x00);
	write(L3GD20_ADDRESS, L3GD20_CTRL_REG1, 0x0F);
	write(LSM303_ACCEL_ADDRESS, LSM303_ACCEL_CTRL_REG4_A, 0 << 4); // Set accelerometers measuring range from -2g to 2g
	write(L3GD20_ADDRESS, L3GD20_CTRL_REG4, 3 << 4); // Set gyroscopes measuring range from -2000°/s to 2000°/s 

	m_attitude = {1, 0, 0, 0};
	m_last_update = micros();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Adafruit9DOFClass::update()
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
		(m_raw_gyro_x - GYRO_X_ZERO) * GYRO_SCALE * dt * 0.5 * radians(2000) / 32768,
		(m_raw_gyro_y - GYRO_Y_ZERO) * GYRO_SCALE * dt * 0.5 * radians(2000) / 32768,
		(m_raw_gyro_z - GYRO_Z_ZERO) * GYRO_SCALE * dt * 0.5 * radians(2000) / 32768
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

void Adafruit9DOFClass::write(uint8_t address, uint8_t reg, uint8_t value) const
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t Adafruit9DOFClass::read(uint8_t address, uint8_t reg) const
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(address, uint8_t(1), uint8_t(true));
	return Wire.read();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Adafruit9DOFClass::get_raw_values()
{
	Wire.beginTransmission(LSM303_ACCEL_ADDRESS);
	Wire.write(LSM303_ACCEL_OUT_X_L_A | 0x80);
	Wire.endTransmission(false);
	Wire.requestFrom(LSM303_ACCEL_ADDRESS, uint8_t(6), uint8_t(true));
	m_raw_accel_x = Wire.read() | Wire.read() << 8;
	m_raw_accel_y = Wire.read() | Wire.read() << 8;
	m_raw_accel_z = Wire.read() | Wire.read() << 8;
	Wire.beginTransmission(L3GD20_ADDRESS);
	Wire.write(L3GD20_OUT_X_L | 0x80);
	Wire.endTransmission(false);
	Wire.requestFrom(L3GD20_ADDRESS, uint8_t(6), uint8_t(true));
	m_raw_gyro_x  = Wire.read() | Wire.read() << 8;
	m_raw_gyro_y  = Wire.read() | Wire.read() << 8;
	m_raw_gyro_z  = Wire.read() | Wire.read() << 8;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
