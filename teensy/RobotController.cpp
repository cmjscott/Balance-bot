#include "RobotController.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::begin(SSC32& ssc, int servo0, int servo1, int servo2, int servo3)
{
	m_ssc = &ssc;
	m_servo0 = &ssc[servo0];
	m_servo1 = &ssc[servo1];
	m_servo2 = &ssc[servo2];
	m_servo3 = &ssc[servo3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::config(float L0, float L1, float L2, float L3, float L4)
{
	m_L0 = L0;
	m_L1 = L1;
	m_L2 = L2;
	m_L3 = L3;
	m_L4 = L4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::get_frame(float& theta0, float& theta1, float& theta2, float& theta3)
{
	theta0 = m_servo0->get_degrees();
	theta1 = m_servo1->get_degrees();
	theta2 = m_servo2->get_degrees();
	theta3 = m_servo3->get_degrees();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::set_frame(float theta0, float theta1, float theta2, float theta3)
{
	m_servo0->set_degrees(theta0);
	m_servo1->set_degrees(theta1);
	m_servo2->set_degrees(theta2);
	m_servo3->set_degrees(theta3);
	forward_kinematics();
	m_ssc->commit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::get_pose(float& x, float& z, float& theta, float &phi)
{
	x = m_x;
	z = m_z;
	theta = m_theta;
	phi = m_phi;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::set_pose(float x, float z, float theta, float phi)
{
	m_x = x;
	m_z = z;
	m_theta = theta;
	m_phi = phi;
	inverse_kinematics();
	m_ssc->commit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::goto_pose(float x, float z, float theta, float phi, unsigned long time)
{
	unsigned long t0 = millis();
	float x0 = m_x;
	float z0 = m_z;
	float theta0 = m_theta;
	float phi0 = m_phi;
	for (unsigned long ti = t0; ti - t0 < time; ti += 20)
	{
		float t = float(ti - t0) / time;
		m_x = t * x + (t - 1) * x0;
		m_z = t * z + (t - 1) * z0;
		m_theta = t * theta + (t - 1) * theta0;
		m_phi = t * phi + (t - 1) * phi0;
		inverse_kinematics();
		m_ssc->commit();
		delay(ti + 20 - millis());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::forward_kinematics()
{
/*	   B         E
	  / \   H   / \
	 /   C--G--D   \
	A               F	*/

	const float theta0 = m_servo0->get_radians();
	const float theta1 = m_servo1->get_radians();
	const float theta2 = m_servo2->get_radians();

	const float xB = -m_L0 / 2 + m_L1 * cos(theta0);
	const float zB = m_L1 * sin(theta0);

	const float xE = m_L0 / 2 - m_L1 * cos(theta1);
	const float zE = m_L1 * sin(theta1);

	const float xD = xE - m_L2 * cos(theta2);
	const float zD = zE - m_L2 * sin(theta2);

	const float xBD = xB - xD;
	const float zBD = zB - zD;

	const float d = sqrt(sq(xBD) + sq(zBD));

	/*
	if this calculates cos(CDB), shouldn't it be divided by (2 * d * L3)?
	c^2 = a^2 + b^2 - 2*a*b*cos(C)  -> looks to be missing the "b" term?
	*/
	const float a = (sq(m_L3) - sq(m_L2) + sq(d)) / (2 * d);


	const float h = sqrt(sq(m_L3) - sq(a));
	const float xC = xD + (a * xBD - h * zBD) / d;
	const float zC = zD + (h * xBD + h * zBD) / d;

	const float xG = (xC + xD) / 2;
	const float zG = (zC + zD) / 2;
	const float xCD = (xD - xC);
	const float zCD = (zD - zC);

	m_theta = -atan2(zCD, xCD);
	m_x = xG + m_L4 * sin(m_theta);
	m_z = zG + m_L4 * cos(m_theta);
	m_phi = m_servo3->get_radians();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotController::inverse_kinematics()
{
/*	   B         E
	  / \   H   / \
	 /   C--G--D   \
	A               F	*/

	const float xG = m_x - m_L4 * sin(m_theta);
	const float zG = m_z - m_L4 * cos(m_theta);

	const float xC = xG - m_L3 / 2 * cos(m_theta);
	const float zC = zG + m_L3 / 2 * sin(m_theta);
	const float xD = xG + m_L3 / 2 * cos(m_theta);
	const float zD = zG - m_L3 / 2 * sin(m_theta);

	const float xAC = xC + m_L0 / 2;
	const float xDF = m_L0 / 2 - xD;

	const float mag_AC = sqrt(sq(xAC) + sq(zC));
	const float arg_AC = atan2(zC, xAC);
	const float mag_DF = sqrt(sq(xDF) + sq(zD));
	const float arg_DF = atan2(zD, xDF);

	const float cos_CAB = (sq(mag_AC) + sq(m_L1) - sq(m_L2)) / (2 * m_L1 * mag_AC);
	const float cos_EFD = (sq(mag_DF) + sq(m_L1) - sq(m_L2)) / (2 * m_L1 * mag_DF);
	const float cos_EDF = (sq(mag_DF) + sq(m_L2) - sq(m_L1)) / (2 * m_L2 * mag_DF);

	m_servo0->set_radians(acos(constrain(cos_CAB, -1, 1)) + arg_AC);
	m_servo1->set_radians(acos(constrain(cos_EFD, -1, 1)) + arg_DF);
	m_servo2->set_radians(acos(constrain(cos_EDF, -1, 1)) - arg_DF);
	m_servo3->set_radians(m_phi);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
