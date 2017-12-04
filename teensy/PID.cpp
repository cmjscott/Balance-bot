#include <Arduino.h>

#include "PID.h"
#include "mathutils.h"


float PID::compute(float setpoint, float input, float timestep, float deriv)
{
	// Compute the error between the current state and the setpoint
	float currentError = setpoint - input;

	// Compute the error integral
	m_errorIntegral += currentError * timestep;
	m_errorIntegral = saturate(m_errorIntegral, m_minOutput / m_Ki, m_maxOutput / m_Ki);

	m_previousError = currentError;

	// Compute the PID controller's output
	float output = m_Kp * currentError + m_Ki * m_errorIntegral - m_Kd * deriv;

	//Serial.print(currentError); Serial.print(" ");
	//Serial.print(m_errorIntegral); Serial.print(" ");
	//Serial.print(deriv); Serial.print(" ");
	//Serial.print(output); Serial.print(" ");
	//Serial.println("");

	//return output;
	return saturate(output, m_minOutput, m_maxOutput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////


float PID::testCompute(float setpoint, float input, float timestep)
{
	float currentError = setpoint - input;

	// Actual PID controller here
	//float errTerm 		= (m_Kp + m_Ki * timestep / 2 + m_Kd / timestep) * currentError;
	//float errTerm_km1 = (-m_Kp + m_Ki * timestep / 2 - 2 * m_Kd / timestep) * m_error_km1;
	//float errTerm_km2 = (m_Kd / timestep) * m_error_km2;
	//float output = m_output_km1 + errTerm + errTerm_km1 + errTerm_km2;

	// just adding a pole
	//float N = 0.25245;
	//float D = 0.7475;
	//float errTerm = (N / timestep) * ((m_Kp * timestep + m_Kd) * currentError - m_Kd * m_error_km1);
	//float outTerm = D * m_output_km1;
	//float output = errTerm + outTerm;

	// Lag yController
	//float Kc 	= 0.36941;
	//float N 	= 0.04279;
	//float D		= 0.6464;

	float Kc 	= 0.34321;
	float N 	= 0.08784;
	float D		= 0.6869;

	//D = .58;

	float A = m_Kp * timestep + m_Kd ;
	float B = -(m_Kd + N * m_Kp * timestep + N * m_Kd);
	float C = m_Kd * N;

	float output = Kc / timestep * (A * currentError + B * m_error_km1 + C * m_error_km2) + D * m_output_km1;

	// Just PD control
	//float output = ((m_Kp * timestep + m_Kd) * currentError - m_Kd * m_error_km1) / timestep;

	m_error_km2 = m_error_km1;
	m_error_km1 = currentError;
	m_output_km1 = output;

	return saturate(output, m_minOutput, m_maxOutput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PID::reset()
{
	m_errorIntegral = 0;
	m_previousError = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PID::load(int address)
{

}

void PID::save(int address) const
{

}
