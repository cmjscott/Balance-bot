#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"

SSC32 ssc;
RobotController robot;

const float X = 0;
const float Y = 100;
const float R = 40; // millimeters
float t = 0;

void setup()
{
	Serial.begin(115200);
	Serial1.begin(115200);
	ssc.begin(Serial1);
	ssc[0].config( 820, 1890,  90, 45);
	ssc[1].config( 820, 1890,  90, 45);
	ssc[2].config(1140, 2500,  90,  0);
	ssc[3].config(1130, 2130, -45, 45);
	robot.begin(ssc, 0, 1, 2, 3);
	robot.config(600, 140, 100, 289, 0); // millimeters
}

void loop()
{
/*	robot.set_pose(X + R * cos(2 * M_PI * t), Y + R * sin(2 * M_PI * t), 0, 0);
	delay(20);
	t += 0.01;*/
	robot.set_pose(0, 50, 0, 0);
	Serial.print(ssc[0].get_degrees()); Serial.print('\t');
	Serial.print(ssc[1].get_degrees()); Serial.print('\t');
	Serial.print(ssc[2].get_degrees()); Serial.print('\n');
	delay(500);
}
