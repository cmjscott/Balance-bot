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
	robot.begin(ssc, 0, 1, 2, 4);
	ssc[0].config( 820, 1440,  90, 45);
	ssc[1].config( 860, 1470,  90, 45);
	ssc[2].config(1340, 2450,  90,  4);
	ssc[4].config(1400, 1850, -22, 22);
	robot.config(600, 140, 100, 337, 0); // millimeters
	robot.home();
}

void loop()
{
/*	robot.set_pose(X + R * cos(2 * M_PI * t), Y + R * sin(2 * M_PI * t), 0, 0);
	delay(20);
	t += 0.01; //*/
/*	robot.goto_pose(0, 50,  5, 0, 200);
	robot.goto_pose(0, 50, -5, 0, 200); //*/
	robot.goto_pose(-30, 20, 0, 0, 200); delay(100);
	robot.goto_pose( 30, 20, 0, 0, 200); delay(100);
	robot.goto_pose( 30, 80, 0, 0, 200); delay(100);
	robot.goto_pose(-30, 80, 0, 0, 200); delay(100); //*/
}
