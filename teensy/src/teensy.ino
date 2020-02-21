#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"
#include "TouchScreen.h"
#include "Controller.h"
#include "PID.h"

PID ctrly, ctrlx;
SSC32 ssc;
RobotController robot;
TouchScreen screen;
Controller controller;

long long t;

// offset for the x angle to make the home position totally flat
// y did not need one
float xOffset = 1.25;

void step_input();
void sine_input();

void setup()
{
	//screen.config xMax, xMin, xLength, yMax, yMin, yLength, N); // meters
	screen.config(3200, 110, 170, 3850, 200, 130, 10); // milimeters
	screen.enable();
	screen.setTimestep(0.005);
	screen.begin();

	// these work remarkabley well.
	//ctrly.setTunings(0.05, 0, 0.03);
	//ctrlx.setTunings(0.05, 0, 0.04);

	ctrly.setOutputLimits(-6, 6);
	ctrlx.setOutputLimits(-10, 10);
	ctrly.setTunings(0.05, 0, 0.035);
	ctrlx.setTunings(0.05, 0, 0.04);

	controller.config(screen, ctrlx, ctrly);
	controller.enable();
	controller.setTimestep(0.01);
	controller.begin();

	controller.setXDesired(15);
	controller.setYDesired(25);

	//Set up robot parameters and attach servos to SSC32
	ssc.begin(Serial1);
	robot.begin(ssc, 0, 1, 2, 4);
	ssc[0].config( 820, 1440,  90, 45);
	ssc[1].config( 800, 1360,  90, 45);
	ssc[2].config(1340, 2450,  90,  5);
	ssc[4].config(1260, 1710, -23, 22);
	robot.config(600, 140, 100, 337, 0); // millimeters

	//Ensure that everything is set up before homing robot
	delay(2000);
	robot.home();
	delay(2000);
}

void loop()
{
	t = millis();

	screen.update();
	controller.update();

	// Set this to true to feed a sine input to the Controller
	// Makes the ball roll in circles
	// Could also use the step input function
	if ((t > 15 * 1000) && false){sine_input();}

	// THIS IS WHERE THE ACTUAL CONTROL HAPPENS
	robot.set_pose(0,30,-controller.getUx() + xOffset, -controller.getUy());
}

void step_input()
{
	controller.setXDesired(-40);
	controller.setYDesired(-20);
}

void sine_input()
{
	controller.setXDesired(50 * sin(1.5 * t/1000) + 15);
	controller.setYDesired(30 * sin(1.5 * t/1000 + 1.6) + 25);
}
