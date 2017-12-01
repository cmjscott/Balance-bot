#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"
#include "TouchScreen.h"
#include "Ycontroller.h"
#include "PID.h"

PID ctrly, ctrlx;
SSC32 ssc;
RobotController robot;
TouchScreen screen;
Ycontroller yController;

float xPos, yPos, dxPos, dyPos;
long long t;
long long tPrev = 0;
int dt;

double u3 = 0;
double u2 = 0;
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
	screen.config(3900, 130, 176.53, 3900, 252, 135.382, 10); // milimeters
	screen.enable();
	screen.setTimestep(0.005);
	screen.begin();

	ctrly.setOutputLimits(-70.0, 70.0);
	ctrlx.setOutputLimits(-70.0,70.0);

	//ctrly.setTunings(-0.04, .001, 0.03);
	//ctrlx.setTunings(-0.05, 0, 0.03);

	ctrly.setTunings(0.05, .001, 0.03);
	ctrlx.setTunings(0.05, .001, 0.03);

	yController.config(screen, ctrlx, ctrly);
	yController.enable();
	yController.setTimestep(0.01);
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
	//delay(50);
	t = millis();

	screen.update();
	yController.update();
	//yController.step(u3, yPos, u3_next);
	//u3 = u3_next;

	/*
	xPos = screen.getX();
	yPos = screen.getY();
	dxPos = screen.getDX();
	dyPos = screen.getDY();

	Serial.print(xPos); Serial.print(" ");
	Serial.print(yPos); Serial.print(" ");
	Serial.print(dxPos); Serial.print(" ");
	Serial.print(dyPos); Serial.print(" ");
	Serial.print((long)t);
	*/

	//yController.update();
	//u3 = yController.getCurrentControl();

	//ssc[4].set_degrees(u3);
	//ssc.commit();

	//u2 = ctrlx.compute(0, xPos, 0.02);
	//Serial.print(" \t | \t ");
	//u3 = ctrly.compute(0, yPos, 0.02);

	//Serial.print(yController.getUy());
	//robot.goto_pose(0, 50,  0, yController.getUy(), 50);
	robot.set_pose(0,50,-yController.getUx(), -yController.getUy());

	//ssc[4].set_degrees(yController.getUy());
	//ssc.commit(20);

	//Serial.print(" -- ");
	//Serial.print(yPos); Serial.print(" ");
	//Serial.print(u3); Serial.print(" ");
	//Serial.print((long)t);
	//Serial.println("");

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
