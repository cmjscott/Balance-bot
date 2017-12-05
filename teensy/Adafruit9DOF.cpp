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
	write(LSM303_MAG_ADDRESS, LSM303_MAG_MR_REG_M, 0x00);
	write(LSM303_ACCEL_ADDRESS, LSM303_ACCEL_CTRL_REG4_A, 0 << 4); // Set accelerometers measuring range from -2g to 2g
	write(L3GD20_ADDRESS, L3GD20_CTRL_REG4, 3 << 4); // Set gyroscopes measuring range from -2000°/s to 2000°/s 
	write(LSM303_MAG_ADDRESS, LSM303_MAG_CRB_REG_M, 1 << 5);

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

	// Calibrate quaternions
	get_accel_vec();
	get_gyro_axis();

	// Compute the estimated attitude using the gyroscopes only
	const Quaternion qdot = m_attitude * m_gyro_axis * dt * 0.5;
	const Quaternion gyro_attitude = (m_attitude + qdot).normalized();

	// Correct the estimated attitude using the accelerometers
	const Quaternion g_U = Quaternion({0, 0, 0, -1}); // Gravity vector in universal frame
	const Quaternion g_E = gyro_attitude.conjugate() * g_U * gyro_attitude; // Gravity in embedded frame
	const Quaternion correction = (Quaternion({1, 0, 0, 0}) - m_accel_vec.normalized() * g_E).conjugate().normalized();
	const Quaternion accel_attitude = gyro_attitude * correction;

	// Merge estimated attitudes using a complementary filter
	const Quaternion new_attitude = lerp(accel_attitude, gyro_attitude, FILTER_PARAM);

	// Update internal quaternions
	m_lin_accel = new_attitude * m_accel_vec * new_attitude.conjugate() - g_U; 
	m_ang_vel = 2 * m_attitude.conjugate() * (new_attitude - m_attitude) / dt;
	m_attitude = new_attitude;
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
	// Read accelerometers
	Wire.beginTransmission(LSM303_ACCEL_ADDRESS);
	Wire.write(LSM303_ACCEL_OUT_X_L_A | 0x80);
	Wire.endTransmission(false);
	Wire.requestFrom(LSM303_ACCEL_ADDRESS, uint8_t(6), uint8_t(true));
	m_raw_accel_x = Wire.read() | Wire.read() << 8;
	m_raw_accel_y = Wire.read() | Wire.read() << 8;
	m_raw_accel_z = Wire.read() | Wire.read() << 8;
	
	// Read gyrometers
	Wire.beginTransmission(L3GD20_ADDRESS);
	Wire.write(L3GD20_OUT_X_L | 0x80);
	Wire.endTransmission(false);
	Wire.requestFrom(L3GD20_ADDRESS, uint8_t(6), uint8_t(true));
	m_raw_gyro_x = Wire.read() | Wire.read() << 8;
	m_raw_gyro_y = Wire.read() | Wire.read() << 8;
	m_raw_gyro_z = Wire.read() | Wire.read() << 8;

	// Read magnetometers
	Wire.beginTransmission(LSM303_MAG_ADDRESS);
	Wire.write(LSM303_MAG_OUT_X_H_M);
	Wire.endTransmission(false);
	Wire.requestFrom(LSM303_MAG_ADDRESS, uint8_t(6), uint8_t(true));
	m_raw_mag_x = Wire.read() | Wire.read() << 8;
	m_raw_mag_y = Wire.read() | Wire.read() << 8;
	m_raw_mag_z = Wire.read() | Wire.read() << 8;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Adafruit9DOFClass::get_accel_vec()
{
	m_accel_vec =
	{
		0,
		((ACCEL_X_MAX + ACCEL_X_MIN) / 2 - m_raw_accel_x) / ((ACCEL_X_MAX - ACCEL_X_MIN) / 2),
		((ACCEL_Y_MAX + ACCEL_Y_MIN) / 2 - m_raw_accel_y) / ((ACCEL_Y_MAX - ACCEL_Y_MIN) / 2),
		((ACCEL_Z_MAX + ACCEL_Z_MIN) / 2 - m_raw_accel_z) / ((ACCEL_Z_MAX - ACCEL_Z_MIN) / 2)
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Adafruit9DOFClass::get_gyro_axis()
{
	m_gyro_axis =
	{
		0,
		(m_raw_gyro_x - GYRO_X_ZERO) * GYRO_SCALE * radians(2000) / 32768,
		(m_raw_gyro_y - GYRO_Y_ZERO) * GYRO_SCALE * radians(2000) / 32768,
		(m_raw_gyro_z - GYRO_Z_ZERO) * GYRO_SCALE * radians(2000) / 32768
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////
