#ifndef __TOUCHSCREEN_H__
#define __TOUCHSCREEN_H__

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_STMPE610.h"





////////////////////////////////////////////////////////////////////////////////////////////////////

class TouchScreen
{
public:
	void config(int xMax, int xMin, float xLength, int yMax, int yMin, float yLength);
	void begin();
	void getPos(float &x, float &y);


private:
	void touchToPos();

	//note: a z measurement is required for the function "readData" to work (adafruit touch sensor library)
	uint16_t m_xMeasure, m_yMeasure;
	uint8_t m_zMeasure;
	int m_xMax, m_xMin, m_yMax, m_yMin;
	float m_xLength, m_yLength, m_xPos, m_yPos;
	Adafruit_STMPE610 touch;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __TOUCHSCREEN_H__
