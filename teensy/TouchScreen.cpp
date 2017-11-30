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

void TouchScreen::config(int xMax, int xMin, float xLength, int yMax, int yMin, float yLength, int N)
{
	m_xMax 		= xMax;
	m_xMin 		= xMin;
	m_xLength = xLength;
	m_yMax 		= yMax;
	m_yMin		= yMin;
	m_yLength = yLength;
	m_N 			= N;

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

void TouchScreen::process(float timestep)
{
	getPos(m_currentX, m_currentY);
	deriv(timestep);

	m_prevX = m_currentX;
	m_prevY = m_currentY;

	Serial.print(m_currentX); Serial.print(" ");
	Serial.print(m_currentY); Serial.print(" ");
	Serial.print(m_avgDX); Serial.print(" ");
	Serial.print(m_avgDY); Serial.print(" ");
	Serial.println(millis());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchScreen::deriv(float dt)
{
	float dx = (m_currentX - m_prevX) / dt;
	float dy = (m_currentY - m_prevY) / dt;

	// calculates the rolling average over the last N samples for dx and dy
	m_avgDX -= m_avgDX / m_N;
	m_avgDX += dx / m_N;

	m_avgDY -= m_avgDY / m_N;
	m_avgDY += dy / m_N;
}
