#include "Ycontroller.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void Ycontroller::config()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::begin()
{
	m_y = 0;
	m_yDot = 0;
	m_theta = 0;
	m_yNext = 0;
	m_thetaNext = 0;
	m_yDotNext = 0;
	tPrev = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::step(double &u3_current, float &y, double &u3_next)
{
	t = millis();
	dt = (t - tPrev) * 0.001;

	m_yNext = -85.0 * m_y + m_yDot + 85.0 * y;
	m_thetaNext = -2140.0 * m_y + u3_current + 2140.0 * y;
	m_yDotNext = -2350.0 * m_y + 98.0 * m_theta + 2350.0 * y;

	m_y = m_y + m_yNext * dt;
	m_theta = m_theta + m_thetaNext * dt;
	m_yDot = m_yDot + m_yDotNext * dt;

	//u3_next = u3_current + (0.0510 * m_y + 6.100 * m_theta + 0.5708 * m_yDot) * dt;

	u3_next = 150 * y;
	tPrev = t;

	//Serial.print(m_yNext); Serial.print(" ");
	//Serial.print(m_thetaNext); Serial.print(" ");
	//Serial.print(m_yDotNext); Serial.print(" ");
//	Serial.print(m_y); Serial.print(" ");
	//Serial.print(m_theta); Serial.print(" ");
	//Serial.print(m_yDot); Serial.print(" ");

	//Serial.println(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
