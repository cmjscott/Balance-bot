#ifndef __ADAFRUIT9DOF_H__
#define __ADAFRUIT9DOF_H__

#include <Arduino.h>
#include <stdint.h>

#include "Quaternion.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

const float FILTER_PARAM = 0.98;

const float ACCEL_X_MINUS_G = -16450;
const float ACCEL_X_PLUS_G  = +15900;
const float ACCEL_Y_MINUS_G = -16500;
const float ACCEL_Y_PLUS_G  = +16000;
const float ACCEL_Z_MINUS_G = -15700;
const float ACCEL_Z_PLUS_G  = +16000;

const float GYRO_X_ZERO =  -3.9;
const float GYRO_Y_ZERO = -52.0;
const float GYRO_Z_ZERO = -13.5;

////////////////////////////////////////////////////////////////////////////////////////////////////

const uint8_t LSM303_ACCEL_ADDRESS = 0x32 >> 1;
const uint8_t LSM303_MAG_ADDRESS   = 0x3C >> 1;
const uint8_t L3GD20_ADDRESS       = 0x6B;

enum // LSM303_ACCEL registers
{
	LSM303_ACCEL_CTRL_REG1_A         = 0x20,   // 00000111   rw
	LSM303_ACCEL_CTRL_REG2_A         = 0x21,   // 00000000   rw
	LSM303_ACCEL_CTRL_REG3_A         = 0x22,   // 00000000   rw
	LSM303_ACCEL_CTRL_REG4_A         = 0x23,   // 00000000   rw
	LSM303_ACCEL_CTRL_REG5_A         = 0x24,   // 00000000   rw
	LSM303_ACCEL_CTRL_REG6_A         = 0x25,   // 00000000   rw
	LSM303_ACCEL_REFERENCE_A         = 0x26,   // 00000000   r
	LSM303_ACCEL_STATUS_REG_A        = 0x27,   // 00000000   r
	LSM303_ACCEL_OUT_X_L_A           = 0x28,
	LSM303_ACCEL_OUT_X_H_A           = 0x29,
	LSM303_ACCEL_OUT_Y_L_A           = 0x2A,
	LSM303_ACCEL_OUT_Y_H_A           = 0x2B,
	LSM303_ACCEL_OUT_Z_L_A           = 0x2C,
	LSM303_ACCEL_OUT_Z_H_A           = 0x2D,
	LSM303_ACCEL_FIFO_CTRL_REG_A     = 0x2E,
	LSM303_ACCEL_FIFO_SRC_REG_A      = 0x2F,
	LSM303_ACCEL_INT1_CFG_A          = 0x30,
	LSM303_ACCEL_INT1_SOURCE_A       = 0x31,
	LSM303_ACCEL_INT1_THS_A          = 0x32,
	LSM303_ACCEL_INT1_DURATION_A     = 0x33,
	LSM303_ACCEL_INT2_CFG_A          = 0x34,
	LSM303_ACCEL_INT2_SOURCE_A       = 0x35,
	LSM303_ACCEL_INT2_THS_A          = 0x36,
	LSM303_ACCEL_INT2_DURATION_A     = 0x37,
	LSM303_ACCEL_CLICK_CFG_A         = 0x38,
	LSM303_ACCEL_CLICK_SRC_A         = 0x39,
	LSM303_ACCEL_CLICK_THS_A         = 0x3A,
	LSM303_ACCEL_TIME_LIMIT_A        = 0x3B,
	LSM303_ACCEL_TIME_LATENCY_A      = 0x3C,
	LSM303_ACCEL_TIME_WINDOW_A       = 0x3D
};

enum // LSM303_MAG registers
{
	LSM303_MAG_CRA_REG_M             = 0x00,
	LSM303_MAG_CRB_REG_M             = 0x01,
	LSM303_MAG_MR_REG_M              = 0x02,
	LSM303_MAG_OUT_X_H_M             = 0x03,
	LSM303_MAG_OUT_X_L_M             = 0x04,
	LSM303_MAG_OUT_Z_H_M             = 0x05,
	LSM303_MAG_OUT_Z_L_M             = 0x06,
	LSM303_MAG_OUT_Y_H_M             = 0x07,
	LSM303_MAG_OUT_Y_L_M             = 0x08,
	LSM303_MAG_SR_REG_Mg             = 0x09,
	LSM303_MAG_IRA_REG_M             = 0x0A,
	LSM303_MAG_IRB_REG_M             = 0x0B,
	LSM303_MAG_IRC_REG_M             = 0x0C,
	LSM303_MAG_TEMP_OUT_H_M          = 0x31,
	LSM303_MAG_TEMP_OUT_L_M          = 0x32
};

enum // L3GD20 registers
{
	L3GD20_WHO_AM_I            = 0x0F,   // 11010100   r
	L3GD20_CTRL_REG1           = 0x20,   // 00000111   rw
	L3GD20_CTRL_REG2           = 0x21,   // 00000000   rw
	L3GD20_CTRL_REG3           = 0x22,   // 00000000   rw
	L3GD20_CTRL_REG4           = 0x23,   // 00000000   rw
	L3GD20_CTRL_REG5           = 0x24,   // 00000000   rw
	L3GD20_REFERENCE           = 0x25,   // 00000000   rw
	L3GD20_OUT_TEMP            = 0x26,   //            r
	L3GD20_STATUS_REG          = 0x27,   //            r
	L3GD20_OUT_X_L             = 0x28,   //            r
	L3GD20_OUT_X_H             = 0x29,   //            r
	L3GD20_OUT_Y_L             = 0x2A,   //            r
	L3GD20_OUT_Y_H             = 0x2B,   //            r
	L3GD20_OUT_Z_L             = 0x2C,   //            r
	L3GD20_OUT_Z_H             = 0x2D,   //            r
	L3GD20_FIFO_CTRL_REG       = 0x2E,   // 00000000   rw
	L3GD20_FIFO_SRC_REG        = 0x2F,   //            r
	L3GD20_INT1_CFG            = 0x30,   // 00000000   rw
	L3GD20_INT1_SRC            = 0x31,   //            r
	L3GD20_TSH_XH              = 0x32,   // 00000000   rw
	L3GD20_TSH_XL              = 0x33,   // 00000000   rw
	L3GD20_TSH_YH              = 0x34,   // 00000000   rw
	L3GD20_TSH_YL              = 0x35,   // 00000000   rw
	L3GD20_TSH_ZH              = 0x36,   // 00000000   rw
	L3GD20_TSH_ZL              = 0x37,   // 00000000   rw
	L3GD20_INT1_DURATION       = 0x38    // 00000000   rw
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class Adafruit9DOFClass
{
public:

	void init();
	void update();

	int16_t get_raw_accel_x() const {return m_raw_accel_x;}
	int16_t get_raw_accel_y() const {return m_raw_accel_y;}
	int16_t get_raw_accel_z() const {return m_raw_accel_z;}
	int16_t get_raw_gyro_x()  const {return m_raw_gyro_x;}
	int16_t get_raw_gyro_y()  const {return m_raw_gyro_y;}
	int16_t get_raw_gyro_z()  const {return m_raw_gyro_z;}

	const Quaternion& get_attitude() const {return m_attitude;}

	float get_roll()  const {return m_attitude.get_roll();}
	float get_pitch() const {return m_attitude.get_pitch();}
	float get_yaw()   const {return m_attitude.get_yaw();}

	inline float get_pitch_approx() const;

private:

	void write(uint8_t address, uint8_t reg, uint8_t value) const;

	uint8_t read(uint8_t address, uint8_t reg) const;

	void get_raw_values();

	int16_t m_raw_accel_x;
	int16_t m_raw_accel_y;
	int16_t m_raw_accel_z;
	int16_t m_raw_gyro_x;
	int16_t m_raw_gyro_y;
	int16_t m_raw_gyro_z;

	Quaternion m_attitude;

	unsigned long m_last_update;

	friend void setup();
	friend void loop();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

float Adafruit9DOFClass::get_pitch_approx() const
{
	// Return the 1st degree Taylor polynomial of the pitch function at the point pitch = 0
	// In other words this computes an approximation of the current attitude pitch without using the
	// asin function
	return 2 * (m_attitude.w * m_attitude.y - m_attitude.z * m_attitude.x);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern Adafruit9DOFClass Adafruit9DOF;

#endif // __ADAFRUIT9DOF_H__
