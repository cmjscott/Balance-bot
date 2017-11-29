#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"
#include "TouchScreen.h"
#include "Ycontroller.h"

SSC32 ssc;
RobotController robot;
TouchScreen screen;
Ycontroller yController;

float xPos = 0;
float yPos = 0;
long long t;
long long tPrev = 0;
int dt;

double u3 = 0;
double u3_next = 0;

const float X = 0;
const float Y = 100;
const float R = 40; // millimeters
//float t = 0;

void setup()
{
	Serial.begin(115200);
	Serial1.begin(115200);

	//screen.config(3900, 130, 0.17653, 3900, 252, 0.135382); // meters
	screen.config(3900, 130, 176.53, 3900, 252, 135.382); // milimeters
	screen.begin();

	yController.begin();

	ssc.begin(Serial1);
	robot.begin(ssc, 0, 1, 2, 4);
	ssc[0].config( 820, 1440,  90, 45);
	ssc[1].config( 860, 1470,  90, 45);
	ssc[2].config(1340, 2450,  90,  4);
	ssc[4].config(1400, 1850, -22, 22);
	robot.config(600, 140, 100, 337, 0); // millimeters
	delay(2000);
	robot.home();
}

void loop()
{
	delay(10);

	t = millis();

	screen.getPos(xPos, yPos);
	yController.step(u3, yPos, u3_next);
	u3 = u3_next;

	ssc[4].set_degrees(u3);
	ssc.commit();

	Serial.print(yPos); Serial.print(" ");
	Serial.print(u3); Serial.print(" ");
	Serial.print((long)t);
	Serial.println("");
	// x,z,theta,phi,time

	//robot.goto_pose(0, 50,  5, 0, 200)



/*	robot.set_pose(X + R * cos(2 * M_PI * t), Y + R * sin(2 * M_PI * t), 0, 0);
	delay(20);
	t += 0.01; //*/
/*	robot.goto_pose(0, 50,  5, 0, 200);
	robot.goto_pose(0, 50, -5, 0, 200); //
	robot.goto_pose(-30, 20, 0, 0, 200); delay(100);
	robot.goto_pose( 30, 20, 0, 0, 200); delay(100);
	robot.goto_pose( 30, 80, 0, 0, 200); delay(100);
	robot.goto_pose(-30, 80, 0, 0, 200); delay(100); */

}
