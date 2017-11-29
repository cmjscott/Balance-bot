#ifndef __YCONTROLLER_H__
#define __YCONTROLLER_H__

#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

class Ycontroller
{
public:
	void config();
	void begin();
	void step(double &u3_current, float &y, double &u3_next);

private:
	double m_y, m_theta, m_yDot;
	double m_yNext, m_thetaNext, m_yDotNext;

	float dt;
	long long tPrev, t;

	float ALC[3][3];
	float B[3];
	float C[3];
	float L[3];
	float K[3];

};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __YCONTROLLER_H__
