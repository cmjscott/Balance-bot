#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"
#include "TouchScreen.h"
#include "Ycontroller.h"
#include "PID.h"

PID ctrly, ctrlx, outerCtrlx, outerCtrly;
SSC32 ssc;
RobotController robot;
TouchScreen screen;
Ycontroller yController;

bool stepFlag = true;

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

// offset for the x angle to make the home position totally float
// y did not need one
float xOffset = 1.25;

void setup()
{
	Serial.begin(115200);
	Serial1.begin(115200);

	//screen.config xMax, xMin, xLength, yMax, yMin, yLength, N); // meters
	screen.config(3200, 110, 170, 3850, 200, 130, 10); // milimeters
	screen.enable();
	screen.setTimestep(0.005);
	screen.begin();

	ctrly.setOutputLimits(-6, 6);
	ctrlx.setOutputLimits(-10, 10);

	// these work remarkabley well.
	//ctrly.setTunings(0.05, 0, 0.03);
	//ctrlx.setTunings(0.05, 0, 0.04);

	outerCtrlx.setTunings(0, 1, 0);
	outerCtrly.setTunings(0, 1, 0);

	ctrly.setTunings(0.05, 0, 0.035);
	ctrlx.setTunings(0.05, 0, 0.04);

	yController.config(screen, ctrlx, ctrly, outerCtrlx, outerCtrly);
	yController.enable();
	yController.setTimestep(0.01);
	yController.begin();

	yController.outerControlFlag = false;

	//yController.setXDesired(60);
	//yController.setYDesired(40);

	yController.setXDesired(10);
	yController.setYDesired(20);

	ssc.begin(Serial1);
	robot.begin(ssc, 0, 1, 2, 4);
	ssc[0].config( 820, 1440,  90, 45);
	ssc[1].config( 800, 1360,  90, 45);
	ssc[2].config(1340, 2450,  90,  5);
	ssc[4].config(1260, 1710, -23, 22);
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


	if ((t > 15 * 1000) && false)
	{
		yController.setXDesired(-40);
		yController.setYDesired(-20);
	}


	// inputs a few sine waves, but leaves me some time to get setup so it isn't
	// jumping all over the fucking place


	if ((t > 10 * 1000) && false)
	{
		yController.setXDesired(50 * sin(1.5 * t/1000) + 10);
		yController.setYDesired(30 * sin(1.5 * t/1000 + 1.6) + 15);
	}


	xPos = screen.getX();
	yPos = screen.getY();
	dxPos = screen.getDX();
	dyPos = screen.getDY();

/*
	Serial.print(xPos); Serial.print(" ");
	Serial.print(-yController.getUx() + xOffset); Serial.print(" ");
	//Serial.print(dxPos); Serial.print(" ");
	Serial.print(yController.getXDesired()); Serial.print(" - ");
	//Serial.print(yController.getInternalXDesired()); Serial.print(" ");

	Serial.print(yPos); Serial.print(" ");
	Serial.print(-yController.getUy()); Serial.print(" ");
	//Serial.print(dyPos); Serial.print(" ");
	Serial.print(yController.getYDesired()); Serial.print(" ");
	//Serial.print(yController.getInternalYDesired()); Serial.print(" ");

	Serial.print((long)t);
	Serial.println("");
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

	// THIS IS WHERE THE ACTUAL CONTROL HAPPENS
	robot.set_pose(0,30,-yController.getUx() + xOffset, -yController.getUy());
	//

	//ssc[4].set_degrees(yController.getUy());
	//ssc.commit(20);

	//Serial.print(" -- ");
	//Serial.print(yPos); Serial.print(" ");
	//Serial.print(u3); Serial.print(" ");
	//Serial.print((long)t);
	//Serial.println("");






}
