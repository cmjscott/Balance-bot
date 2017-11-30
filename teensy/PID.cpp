#include <Arduino.h>

#include "PID.h"
#include "mathutils.h"


float PID::compute(float setpoint, float input, float timestep)
{
	// Compute the error between the current state and the setpoint
	float currentError = setpoint - input;

	// Compute the error integral
	m_errorIntegral += currentError * timestep;
	m_errorIntegral = saturate(m_errorIntegral, m_minOutput / m_Ki, m_maxOutput / m_Ki);

	// Compute the error derivative
	float errorDerivative = (currentError - m_previousError) / timestep;
	errorDerivative = squash(errorDerivative,-10, 10);
	errorDerivative = saturate(errorDerivative,-400, 400);

	m_previousError = currentError;

	// Compute the PID controller's output
	float output = m_Kp * currentError + m_Ki * m_errorIntegral - m_Kd * errorDerivative;

	Serial.print(currentError); Serial.print("\t");
	Serial.print(m_errorIntegral); Serial.print("\t");
	Serial.print(errorDerivative); Serial.print("\t");
	Serial.print(output); Serial.print(" ");
	//Serial.println("");

	//return output;
	return saturate(output, m_minOutput, m_maxOutput);


}

void PID::reset()
{
	m_errorIntegral = 0;
	m_previousError = 0;
}

void PID::load(int address)
{

}

void PID::save(int address) const
{

}
