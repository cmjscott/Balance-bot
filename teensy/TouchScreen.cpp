#include "TouchScreen.h"

/*
#define X_MAX 3900
#define X_MIN 130
#define X_LENGTH 176.53 // milimeters, 6.95 inches

#define Y_MAX 3900
#define Y_MIN 252
#define Y_LENGTH 135.382 // milimeters, 5.33 inches
*/
////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchScreen::config(int xMax, int xMin, float xLength, int yMax, int yMin, float yLength)
{
	m_xMax 		= xMax;
	m_xMin 		= xMin;
	m_xLength = xLength;
	m_yMax 		= yMax;
	m_yMin		= yMin;
	m_yLength = yLength;

	touch = Adafruit_STMPE610();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchScreen::begin()
{
	touch.begin(0x41);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchScreen::getPos(float &x, float &y)
{
	if (touch.touched())
	{
    while (! touch.bufferEmpty()) {
      touch.readData(&m_xMeasure, &m_yMeasure, &m_zMeasure);
			touchToPos();
    }
    touch.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
		x = m_xPos;
		y = m_yPos;
	}
	else
	{
		//x = 0;
		//y = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchScreen::touchToPos()
{
	m_xPos = ((float)m_xMeasure - (m_xMax - m_xMin) / 2) * m_xLength / (m_xMax - m_xMin);
	m_yPos = ((float)m_yMeasure - (m_yMax - m_yMin) / 2) * m_yLength / (m_yMax - m_yMin);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
