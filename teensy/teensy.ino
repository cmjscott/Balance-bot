#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"
#include "TouchScreen.h"
#include "Controller.h"
#include "PID.h"

PID ctrly, ctrlx, outerCtrlx, outerCtrly;
SSC32 ssc;
RobotController robot;
TouchScreen screen;
Controller controller;

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
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
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

	controller.config(screen, ctrlx, ctrly, outerCtrlx, outerCtrly);
	controller.enable();
	controller.setTimestep(0.01);
	controller.begin();

	controller.outerControlFlag = false;

	//controller.setXDesired(60);
	//controller.setYDesired(40);

	controller.setXDesired(15);
	controller.setYDesired(25);

	ssc.begin(Serial1);
	robot.begin(ssc, 0, 1, 2, 4);
	ssc[0].config( 820, 1440,  90, 45);
	ssc[1].config( 800, 1360,  90, 45);
	ssc[2].config(1340, 2450,  90,  5);
	ssc[4].config(1260, 1710, -23, 22);
	robot.config(600, 140, 100, 337, 0); // millimeters
	delay(2000);
	robot.home();
	delay(200000);
}

void loop()
{
	while (true)
	{
		robot.goto_pose(0, 50, -10, 0, 100);
		robot.goto_pose(0, 50, -10, -20, 200); delay(50);
		robot.goto_pose(0, 50, -10, 20, 200); delay(50);
		robot.goto_pose(0, 50, -10, -20, 200); delay(50);
		robot.goto_pose(0, 50, -10, 20, 200); delay(50);
		robot.goto_pose(0, 50, 0, 0, 100);
		robot.goto_pose(0, 50, 10, 0, 100);
		robot.goto_pose(0, 50, 10, -20, 200); delay(50);
		robot.goto_pose(0, 50, 10, 20, 200); delay(50);
		robot.goto_pose(0, 50, 10, -20, 200); delay(50);
		robot.goto_pose(0, 50, 10, 20, 200); delay(50);
		robot.goto_pose(0, 50, 0, 0, 100);
	}

	//delay(50);
	t = millis();

	screen.update();
	controller.update();
	//controller.step(u3, yPos, u3_next);
	//u3 = u3_next;


	if ((t > 15 * 1000) && false)
	{
		controller.setXDesired(-40);
		controller.setYDesired(-20);
	}


	// inputs a few sine waves, but leaves me some time to get setup so it isn't
	// jumping all over the fucking place


	if ((t > 10 * 1000) && true)
	{
		controller.setXDesired(50 * sin(1.5 * t/1000) + 15);
		controller.setYDesired(30 * sin(1.5 * t/1000 + 1.6) + 25);
	}


	xPos = screen.getX();
	yPos = screen.getY();
	dxPos = screen.getDX();
	dyPos = screen.getDY();

/*
	Serial.print(xPos); Serial.print(" ");
	Serial.print(-controller.getUx() + xOffset); Serial.print(" ");
	//Serial.print(dxPos); Serial.print(" ");
	Serial.print(controller.getXDesired()); Serial.print(" - ");
	//Serial.print(controller.getInternalXDesired()); Serial.print(" ");

	Serial.print(yPos); Serial.print(" ");
	Serial.print(-controller.getUy()); Serial.print(" ");
	//Serial.print(dyPos); Serial.print(" ");
	Serial.print(controller.getYDesired()); Serial.print(" ");
	//Serial.print(controller.getInternalYDesired()); Serial.print(" ");

	Serial.print((long)t);
	Serial.println("");
*/

	//controller.update();
	//u3 = controller.getCurrentControl();

	//ssc[4].set_degrees(u3);
	//ssc.commit();

	//u2 = ctrlx.compute(0, xPos, 0.02);
	//Serial.print(" \t | \t ");
	//u3 = ctrly.compute(0, yPos, 0.02);

	//Serial.print(controller.getUy());
	//robot.goto_pose(0, 50,  0, controller.getUy(), 50);

	// THIS IS WHERE THE ACTUAL CONTROL HAPPENS
	robot.set_pose(0,30,-controller.getUx() + xOffset, -controller.getUy());
	//

	//ssc[4].set_degrees(controller.getUy());
	//ssc.commit(20);

	//Serial.print(" -- ");
	//Serial.print(yPos); Serial.print(" ");
	//Serial.print(u3); Serial.print(" ");
	//Serial.print((long)t);
	//Serial.println("");






}
