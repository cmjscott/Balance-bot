#ifndef __TOUCHSCREEN_H__
#define __TOUCHSCREEN_H__

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_STMPE610.h"
#include "PeriodicProcess.h"






////////////////////////////////////////////////////////////////////////////////////////////////////

class TouchScreen : public PeriodicProcess
{
public:
	void config(int xMax, int xMin, float xLength, int yMax, int yMin, float yLength, int N);
	void begin();
	void getPos(float &x, float &y);

	float getX(){return m_currentX;}
	float getY(){return m_currentY;}
	float getDX(){return m_avgDX;}
	float getDY(){return m_avgDY;}



protected:
	void touchToPos();
	virtual void process(float timestep);
	void deriv(float dt);


	//note: a z measurement is required for the function "readData" to work (adafruit touch sensor library)

	uint16_t m_xMeasure, m_yMeasure;
	uint8_t m_zMeasure;

	float m_currentX, m_currentY, m_prevX, m_prevY, m_avgDX, m_avgDY;

	int m_xMax, m_xMin, m_yMax, m_yMin, m_N;
	float m_xLength, m_yLength, m_xPos, m_yPos;

	bool m_resetFlag = false;

	Adafruit_STMPE610 touch;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __TOUCHSCREEN_H__
