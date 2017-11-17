#include <Arduino.h>
#include "SSC32.h"
#include "RobotController.h"

SSC32 ssc;
SSC32::Servo& twist   = ssc[0];
SSC32::Servo& gripper = ssc[1];

void setup()
{
	Serial1.begin(115200);
	ssc.begin(Serial1);
}

void loop()
{
	twist.set_degrees(45);
	gripper.set_degrees(45);
	ssc.commit(2000);
	while (!ssc.is_done()) delay(10);

	twist.set_position(2000);
	gripper.set_position(2000);
	ssc.commit(1000);
	while (!ssc.is_done()) delay(10);
}
