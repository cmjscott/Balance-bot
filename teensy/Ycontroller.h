#ifndef __YCONTROLLER_H__
#define __YCONTROLLER_H__

#include <Arduino.h>

#include "PeriodicProcess.h"
#include "TouchScreen.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

class Ycontroller : public PeriodicProcess
{
public:
	void config(TouchScreen& _screen);
	void begin();
	void reset();
	void setTarget(double _target) {target = _target;}
	//void step(double &u3_current, float &y, double &u3_next);

	double getCurrentControl() const {return u;}

protected:
	virtual void process(float timestep);

	double u;
	
	double uk, ukm1, ukm2, ukm3, ukm4;
	double ek, ekm1, ekm2, ekm3;

	double ku, kum1, kum2, kum3, kum4;
	double ke, kem1, kem2, kem3;

	TouchScreen *screen;

	double target;

	float xPos, yPos;

	//double m_y, m_theta, m_yDot;
	//double m_yNext, m_thetaNext, m_yDotNext;

	float dt;
	long long tPrev, t;

	/*
	float ALC[3][3];
	float B[3];
	float C[3];
	float L[3];
	float K[3];
	*/
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __YCONTROLLER_H__
