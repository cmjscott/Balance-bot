#ifndef __TOUCHSCREEN_H__
#define __TOUCHSCREEN_H__

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_STMPE610.h"
#include "PeriodicProcess.h"
#include "Clock.h"

#define SGFILTER_NP 15

////////////////////////////////////////////////////////////////////////////////////////////////////

class TouchScreen : public PeriodicProcess
{
public:
	void config(int xMax, int xMin, float xLength, int yMax, int yMin, float yLength, int N);
	void begin();
	void getPos(float &x, float &y);
	void setTimeout(float tm) {m_timeout = tm * 1000000;}

	float getX(){return applySGFilter(m_xSamples, SGFILTER_COEFF0);}
	float getY(){return applySGFilter(m_ySamples, SGFILTER_COEFF0);}
	float getDX(){return applySGFilter(m_xSamples, SGFILTER_COEFF1) / getTimestep();}
	float getDY(){return applySGFilter(m_ySamples, SGFILTER_COEFF1) / getTimestep();}
	float getXMeasure(){return m_xMeasure;}
	float getYMeasure(){return m_yMeasure;}

	bool m_noTouchFlag = false;
	bool m_resetFlag = false;

protected:
	void touchToPos();
	virtual void process(float timestep);
	void deriv(float dt);


	//note: a z measurement is required for the function "readData" to work (adafruit touch sensor library)

	uint16_t m_xMeasure, m_yMeasure;
	uint8_t m_zMeasure;

	float m_xSamples[SGFILTER_NP];
	float m_ySamples[SGFILTER_NP];

	int m_xMax, m_xMin, m_yMax, m_yMin, m_N;
	float m_xLength, m_yLength, m_xPos, m_yPos;

	float m_timeout = 1000000;




	Adafruit_STMPE610 touch;
	Clock m_clock;

	static float applySGFilter(const float samples[], const float coeffs[]);

	static const float SGFILTER_COEFF0[SGFILTER_NP];
	static const float SGFILTER_COEFF1[SGFILTER_NP];
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __TOUCHSCREEN_H__
