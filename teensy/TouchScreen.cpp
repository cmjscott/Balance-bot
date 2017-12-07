#include "TouchScreen.h"

/*
#define X_MAX 3900
#define X_MIN 130
#define X_LENGTH 176.53 // milimeters, 6.95 inches

#define Y_MAX 3900
#define Y_MIN 252
#define Y_LENGTH 135.382 // milimeters, 5.33 inches
*/

const float TouchScreen::SGFILTER_COEFF0[] =
{
	-78.0 / 1105,
	-13.0 / 1105,
   42.0 / 1105,
	 87.0 / 1105,
	122.0 / 1105,
	147.0 / 1105,
	162.0 / 1105,
	167.0 / 1105,
	162.0 / 1105,
	147.0 / 1105,
	122.0 / 1105,
	 87.0 / 1105,
   42.0 / 1105,
	-13.0 / 1105,
	-78.0 / 1105,
};

const float TouchScreen::SGFILTER_COEFF1[] =
{
	-7.0 / 280,
	-6.0 / 280,
	-5.0 / 280,
	-4.0 / 280,
	-3.0 / 280,
	-2.0 / 280,
	-1.0 / 280,
	 0.0 / 280,
	 1.0 / 280,
	 2.0 / 280,
	 3.0 / 280,
	 4.0 / 280,
	 5.0 / 280,
	 6.0 / 280,
	 7.0 / 280,
};

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
	if (touch.touched() && ~m_noTouchFlag)
	{
    while (! touch.bufferEmpty()) {
      touch.readData(&m_xMeasure, &m_yMeasure, &m_zMeasure);
			touchToPos();
    }
    touch.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
		x = m_xPos;
		y = m_yPos;
		m_clock.restart();
	}
	else
	{
		if (m_clock.getElapsedTime() > m_timeout)
		{
			m_noTouchFlag = true;
		}
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
	for (int i = 0; i < SGFILTER_NP - 1; i++)
	{
		m_xSamples[i] = m_xSamples[i+1];
		m_ySamples[i] = m_ySamples[i+1];
	}
	getPos(m_xSamples[SGFILTER_NP - 1], m_ySamples[SGFILTER_NP - 1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

float TouchScreen::applySGFilter(const float samples[], const float coeffs[])
{
	float output = 0;
	for (int i = 0; i < SGFILTER_NP; i++)
		output += samples[i] * coeffs[i];
	return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
