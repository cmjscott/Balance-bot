#include "Ycontroller.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void Ycontroller::config(TouchScreen& _screen, PID& _PIDx, PID& _PIDy)
{
	screen 	= &_screen;
	ctrlx		= &_PIDx;
	ctrly		= &_PIDy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::begin()
{
	ku 		= 0.7723;
	kum1	= -2.969;
	kum2 	= 7.594;
	kum3	= -3.406;
	kum4 	= -0.4491;

	ke 		= 1;
	kem1 	= -2.414;
	kem2 	= 1.903;
	kem3 	= -0.489;

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::reset()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Ycontroller::process(float timestep)
{
	ux = ctrlx->compute2(-10,screen->getX(),timestep,screen->getDX());
	uy = ctrly->compute2(0,screen->getY(),timestep,screen->getDY());
	/*
	screen->getPos(xPos, yPos);

	ek 	= yPos - target;

	uk 	= ke 		* ek 		+ kem1 * ekm1 	+ kem2 * ekm2 	+ kem3 * ekm3;
	uk += kum1 	* ukm1 	+ kum2 * ukm2 	+ kum3 * ukm3 	+ kum4 * ukm4;
	uk 	= (uk / ku);

	u  += uk * timestep;

	ekm3 = ekm2;
	ekm2 = ekm1;
	ekm1 = ek;

	ukm4 = ukm3;
	ukm3 = ukm2;
	ukm2 = ukm1;
	ukm1 = uk;
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
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
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
