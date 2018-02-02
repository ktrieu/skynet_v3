#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    batt,           sensorAnalog)
#pragma config(Sensor, in2,    mogoPotRight,   sensorPotentiometer)
#pragma config(Sensor, in7,    mogoPotLeft,    sensorPotentiometer)
#pragma config(Sensor, in8,    clawPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  green,          sensorLEDtoVCC)
#pragma config(Sensor, dgtl2,  yellow,         sensorLEDtoVCC)
#pragma config(Sensor, dgtl3,  red,            sensorLEDtoVCC)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_6,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_7,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_8,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           rightFront,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightRear,     tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           claw,          tmotorVex393_MC29, openLoop, encoderPort, I2C_8)
#pragma config(Motor,  port4,           mogoRight,     tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port5,           armRight,      tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port6,           armLeft,       tmotorVex393_MC29, openLoop, encoderPort, I2C_5)
#pragma config(Motor,  port7,           mogoLeft,      tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_4)
#pragma config(Motor,  port8,           clawRot,       tmotorVex393_MC29, openLoop, encoderPort, I2C_7)
#pragma config(Motor,  port9,           leftRear,      tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_6)
#pragma config(Motor,  port10,          leftFront,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define DR_AUTO_LOG

#include "lib/joy.h"
#include "lib/drive.h"
#include "lib/pid.h"
#include "lib/arm.h"*
#include "lib/claw.h"
#include "lib/mogo.h"

ArmData armData;
ClawData clawData;

MogoData mogoDataLeft;
MogoData mogoDataRight;

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

void update_battery()
{
	int bValue = SensorValue[batt];
	float voltage = bValue / 155;
	if (voltage < 5.5)
	{
		SensorValue[red] = 1;
		SensorValue[yellow] = 0;
		SensorValue[green] = 0;
	}
	else if (voltage < 6.5)
	{
		SensorValue[red] = 0;
		SensorValue[yellow] = 1;
		SensorValue[green] = 0;
	}
	else
	{
		SensorValue[red] = 0;
		SensorValue[yellow] = 0;
		SensorValue[green] = 1;
	}
}

void init()
{
	datalogClear();
	joy_init();
	dr_init();
	arm_init(armData);
	claw_init(clawData);
	mogo_init(mogoDataLeft, mogoLeft);
	mogo_init(mogoDataRight, mogoRight);
}

void pre_auton()
{
  bStopTasksBetweenModes = true;
}

task autonomous()
{
	//init();

}

task usercontrol()
{
	init();
	clearDebugStream();
	while (true)
	{
		joy_poll_input();
		update_battery();
		arm_update(armData);
		dr_update();
		claw_update(clawData);
		mogo_update(mogoDataLeft);
		mogo_update(mogoDataRight);
		wait1Msec(20);
	}
}
