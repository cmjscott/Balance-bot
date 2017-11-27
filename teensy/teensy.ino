#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"
#include "Adafruit9DOF.h"

void setup()
{
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	Serial.begin(115200);
	Adafruit9DOF.init();
}

void loop()
{
	Adafruit9DOF.update();
/*	Serial.print(Adafruit9DOF.get_roll());  Serial.print('\t');
	Serial.print(Adafruit9DOF.get_pitch()); Serial.print('\t');
	Serial.print(Adafruit9DOF.get_yaw());   Serial.print('\n');
	delay(5); //*/
	Serial.print(micros()); Serial.print(',');
	Serial.print(Adafruit9DOF.get_lin_accel().x, 6), Serial.print(',');
	Serial.print(Adafruit9DOF.get_lin_accel().y, 6), Serial.print(',');
	Serial.print(Adafruit9DOF.get_lin_accel().z, 6), Serial.print(',');
	Serial.print(Adafruit9DOF.get_ang_vel().x, 6), Serial.print(',');
	Serial.print(Adafruit9DOF.get_ang_vel().y, 6), Serial.print(',');
	Serial.print(Adafruit9DOF.get_ang_vel().z, 6), Serial.print('\n');
	delay(5); //*/
}
