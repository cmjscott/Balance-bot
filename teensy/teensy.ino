#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"

SSC32 ssc;
RobotController robot;

const float X = 0;
const float Y = 150;
const float R = 100; // millimeters
float t = 0;

void setup()
{
	Serial1.begin(115200);
	ssc.begin(Serial1);
	ssc[0].config( 870, 1370,  90, 45);
	ssc[1].config(1240, 1740,  90, 45);
	ssc[2].config(1820, 2500,  45,  0);
	ssc[3].config(1130, 2130, -45, 45);
	robot.begin(ssc, 0, 1, 2, 3);
	robot.config(/* TODO */); // millimeters
}

void loop()
{
	robot.set_pose(X + R * cos(2 * M_PI * t), Y + R * sin(2 * M_PI * t), 0, 0);
	delay(100);
	t += 0.1;
}
