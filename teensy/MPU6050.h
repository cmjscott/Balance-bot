#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <Arduino.h>
#include <stdint.h>

#include "Quaternion.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

const float FILTER_PARAM = 0.98;

const float ACCEL_X_MINUS_G = -15900;
const float ACCEL_X_PLUS_G  = +16700;
const float ACCEL_Y_MINUS_G = -16700;
const float ACCEL_Y_PLUS_G  = +16050;
const float ACCEL_Z_MINUS_G = -18400;
const float ACCEL_Z_PLUS_G  = +14800;

const float GYRO_X_ZERO =  -2.65;
const float GYRO_Y_ZERO =   2.46;
const float GYRO_Z_ZERO = -57.82;

////////////////////////////////////////////////////////////////////////////////////////////////////

const uint8_t MPU6050_ADDRESS = 0x68;

enum // MPU registers
{
	SELF_TEST_X        = 0x0D,
	SELF_TEST_Y        = 0x0E,
	SELF_TEST_Z        = 0x0F,
	SELF_TEST_A        = 0x10,
	CONFIG             = 0x1A,
	GYRO_CONFIG        = 0x1B,
	ACCEL_CONFIG       = 0x1C,
	FIFO_EN            = 0x23,
	I2C_MST_CTRL       = 0x24,
	I2C_SLV0_ADDR      = 0x25,
	I2C_SLV0_REG       = 0x26,
	I2C_SLV0_CTRL      = 0x27,
	I2C_SLV1_ADDR      = 0x28,
	I2C_SLV1_REG       = 0x29,
	I2C_SLV1_CTRL      = 0x2A,
	I2C_SLV2_ADDR      = 0x2B,
	I2C_SLV2_REG       = 0x2C,
	I2C_SLV2_CTRL      = 0x2D,
	I2C_SLV3_ADDR      = 0x2E,
	I2C_SLV3_REG       = 0x2F,
	I2C_SLV3_CTRL      = 0x30,
	I2C_SLV4_ADDR      = 0x31,
	I2C_SLV4_REG       = 0x32,
	I2C_SLV4_DO        = 0x33,
	I2C_SLV4_CTRL      = 0x34,
	I2C_SLV4_DI        = 0x35,
	I2C_MST_STATUS     = 0x36,
	INT_PIN_CFG        = 0x37,
	INT_ENABLE         = 0x38,
	INT_STATUS         = 0x3A,
	ACCEL_XOUT_H       = 0x3B,
	ACCEL_XOUT_L       = 0x3C,
	ACCEL_YOUT_H       = 0x3D,
	ACCEL_YOUT_L       = 0x3E,
	ACCEL_ZOUT_H       = 0x3F,
	ACCEL_ZOUT_L       = 0x40,
	TEMP_OUT_H         = 0x41,
	TEMP_OUT_L         = 0x42,
	GYRO_XOUT_H        = 0x43,
	GYRO_XOUT_L        = 0x44,
	GYRO_YOUT_H        = 0x45,
	GYRO_YOUT_L        = 0x46,
	GYRO_ZOUT_H        = 0x47,
	GYRO_ZOUT_L        = 0x48,
	EXT_SENS_DATA_00   = 0x49,
	EXT_SENS_DATA_01   = 0x4A,
	EXT_SENS_DATA_02   = 0x4B,
	EXT_SENS_DATA_03   = 0x4C,
	EXT_SENS_DATA_04   = 0x4D,
	EXT_SENS_DATA_05   = 0x4E,
	EXT_SENS_DATA_06   = 0x4F,
	EXT_SENS_DATA_07   = 0x50,
	EXT_SENS_DATA_08   = 0x51,
	EXT_SENS_DATA_09   = 0x52,
	EXT_SENS_DATA_10   = 0x53,
	EXT_SENS_DATA_11   = 0x54,
	EXT_SENS_DATA_12   = 0x55,
	EXT_SENS_DATA_13   = 0x56,
	EXT_SENS_DATA_14   = 0x57,
	EXT_SENS_DATA_15   = 0x58,
	EXT_SENS_DATA_16   = 0x59,
	EXT_SENS_DATA_17   = 0x5A,
	EXT_SENS_DATA_18   = 0x5B,
	EXT_SENS_DATA_19   = 0x5C,
	EXT_SENS_DATA_20   = 0x5D,
	EXT_SENS_DATA_21   = 0x5E,
	EXT_SENS_DATA_22   = 0x5F,
	EXT_SENS_DATA_23   = 0x60,
	I2C_SLV0_DO        = 0x63,
	I2C_SLV1_DO        = 0x64,
	I2C_SLV2_DO        = 0x65,
	I2C_SLV3_DO        = 0x66,
	I2C_MST_DELAY_CTRL = 0x67,
	SIGNAL_PATH_RESET  = 0x68,
	USER_CTRL          = 0x6A,
	PWR_MGMT_1         = 0x6B,
	PWR_MGMT_2         = 0x6C,
	FIFO_COUNT_H       = 0x72,
	FIFO_COUNT_L       = 0x73,
	FIFO_R_W           = 0x74,
	WHO_AM_I           = 0x75
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class MPU6050Class
{
public:

	void init();
	void update();

	int16_t get_raw_accel_x() const {return m_raw_accel_x;}
	int16_t get_raw_accel_y() const {return m_raw_accel_y;}
	int16_t get_raw_accel_z() const {return m_raw_accel_z;}
	int16_t get_raw_temp()    const {return m_raw_temp;}
	int16_t get_raw_gyro_x()  const {return m_raw_gyro_x;}
	int16_t get_raw_gyro_y()  const {return m_raw_gyro_y;}
	int16_t get_raw_gyro_z()  const {return m_raw_gyro_z;}

	const Quaternion& get_attitude() const {return m_attitude;}

	float get_roll()  const {return m_attitude.get_roll();}
	float get_pitch() const {return m_attitude.get_pitch();}
	float get_yaw()   const {return m_attitude.get_yaw();}

	inline float get_pitch_approx() const;

private:

	void write(uint8_t address, uint8_t value) const;

	uint8_t read(uint8_t address) const;

	void get_raw_values();

	int16_t m_raw_accel_x;
	int16_t m_raw_accel_y;
	int16_t m_raw_accel_z;
	int16_t m_raw_temp;
	int16_t m_raw_gyro_x;
	int16_t m_raw_gyro_y;
	int16_t m_raw_gyro_z;

	Quaternion m_attitude;

	unsigned long m_last_update;

	friend void setup();
	friend void loop();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

float MPU6050Class::get_pitch_approx() const
{
	// Return the 1st degree Taylor polynomial of the pitch function at the point pitch = 0
	// In other words this computes an approximation of the current attitude pitch without using the
	// asin function
	return 2 * (m_attitude.w * m_attitude.y - m_attitude.z * m_attitude.x);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern MPU6050Class MPU6050;

#endif // __MPU6050_H__
