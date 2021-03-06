#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    driveGyro,      sensorGyro)
#pragma config(Sensor, dgtl1,  liftBump1,      sensorTouch)
#pragma config(Sensor, dgtl2,  liftBump2,      sensorTouch)
#pragma config(Sensor, dgtl3,  stackSonar,     sensorSONAR_raw)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           mobileLeft,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           lift3,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           rightDrive1,   tmotorVex393_MC29, openLoop, reversed, driveLeft, encoderPort, I2C_1)
#pragma config(Motor,  port4,           rightDrive2,   tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port5,           roller,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           lift12,        tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port7,           leftDrive1,    tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port8,           leftDrive2,    tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port9,           cone,          tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_3)
#pragma config(Motor,  port10,          mobileRight,   tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

//Sensor definitions
#define liftPos nMotorEncoder[lift12]
#define liftReset1 SensorValue[liftBump1]
#define liftReset2 SensorValue[liftBump2]
#define intakePos nMotorEncoder[cone]
#define driveEncoder nMotorEncoder[rightDrive1]
#define gyroPos SensorValue(driveGyro)
#define stackDistance SensorValue[stackSonar]
#define rightJoyRaw vexRT[Ch1]
#define leftJoyRaw vexRT[Ch3]
#define isAuton bIfiAutonomousMode
#define btnPressed (vexRT[Btn6U] || vexRT[Btn6D] || vexRT[Btn5U] ||	vexRT[Btn5D] || vexRT[Btn8R] || vexRT[Btn7U] ||vexRT[Btn7D])

bool autoStack = false;

//Included files
#include "Utility Functions.h"
#include "Drive Control.h"
#include "Lift Control.h"
#include "Intake Control.h"
#include "Mobile Control.h"
#include "Autons.h"
#include "Auton Execution.h"
#include "LCDcontrol.h"

void stackCone() {
	if (stackDistance == -1) {
		return;
	}

	int originalHeight = liftPos + 200;

	rollerIn(30);

	clearTimer(T1);
	liftUp(100);
	while(time1[T1] < 150) {
		if (btnPressed || stackDistance == -1) {
			liftStop();
			return;
		}
		if (stackDistance < 1300) {
			clearTimer(T1);
		}
		wait1Msec(3);
		//second = first;
		//first = stackDistance;
	}

	//bring intake arm back
	liftUp(-10);
	intakeUp(120);
	clearTimer(T1);
	waitUntil(time1(T1) > 500 || btnPressed);
	liftStop();
	intakeStop();
	if (btnPressed || stackDistance == -1) {
		return;
	}

	//wait for arm to settle
	clearTimer(T1);
	waitUntil(time1(T1) > 200 || btnPressed);
	if (btnPressed) {
		return;
	}

	//drop cone
	rollerOut(120);
	liftUp(20);
	clearTimer(T1);
	waitUntil(time1(T1) > 300 || btnPressed);
	if (btnPressed) {
		return;
	}

	//bring intake arm forward
	intakeDown(120);
	clearTimer(T1);
	waitUntil(time1(T1) > 500 || btnPressed);
	if (btnPressed) {
		return;
	}
	intakeStop();

	//bring lift down
	liftUp(-100);
	rollerIn(30);
	waitUntil(liftPos < originalHeight || btnPressed);
	if (btnPressed) {
		return;
	}
	liftStop();
}

void pre_auton() {
	startTask(LCD);
	initializeSensors();
}

task autonomous() {
	startTask(liftControl);
	startTask(intakeControl);
	startTask(rollerControl);
	startTask(driveControl);
	startTask(LCD);
	autonExecution(getAuto());
}

task usercontrol() {
	startTask(liftControl);
	startTask(intakeControl);
	startTask(rollerControl);
	startTask(driveControl);
	startTask(LCD);
	while(true) {
		manualMobile();
		if (vexRT[Btn8L]) {
			autoStack = true;
		}
		if (autoStack) {
			stackCone();
			autoStack = false;
		}
	}
}
