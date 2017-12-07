#ifndef __YCONTROLLER_H__
#define __YCONTROLLER_H__

#include <Arduino.h>

#include "PeriodicProcess.h"
#include "TouchScreen.h"
#include "PID.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

class Ycontroller : public PeriodicProcess
{
public:
	void config(TouchScreen& _screen, PID& _PIDx, PID& _PIDy);
	void begin();
	void reset();

	double getUx() const {return ux;}
	double getUy() const {return uy;}

	void setXDesired(float xd) {xDesired = xd;}
	void setYDesired(float yd) {yDesired = yd;}

	float getXDesired() const {return xDesired;}
	float getYDesired() const {return yDesired;}

protected:
	virtual void process(float timestep);

	float ux, uy, xDesired, yDesired;

	TouchScreen *screen;
	PID *ctrlx, *ctrly;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __YCONTROLLER_H__
