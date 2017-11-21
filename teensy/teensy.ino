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
	Serial.print(Adafruit9DOF.get_roll());  Serial.print('\t');
	Serial.print(Adafruit9DOF.get_pitch()); Serial.print('\t');
	Serial.print(Adafruit9DOF.get_yaw());   Serial.print('\n');
	delay(5);
}
