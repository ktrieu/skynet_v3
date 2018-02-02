#include "lib/util.h"
#include "lib/auto.h"

#define BRAKE_PWR -10
#define BRAKE_TIME_MSEC 400

#define AUTO_DRIVE_FAC 1

#define TICKS_PER_CM 6

void dr_init()
{
	nMotorEncoder[rightRear] = 0;
	nMotorEncoder[leftRear] = 0;
}

bool dr_at_target(int target, int encoder)
{
	return abs(target) < abs(encoder);
}

void dr_set_left(int val)
{
	motor[leftRear] = motor[leftFront] = val;
}

void dr_set_right(int val)
{
	motor[rightRear] = motor[rightFront] = val;
}

void dr_drive(int forward, int rot)
{
		int left = forward - rot;
		int right = forward + rot;
		left = clamp_motor(left);
		right = clamp_motor(right);
		dr_set_left(left);
		dr_set_right(right);
}

void dr_drive_auto(int dist, int pwr = 65)
{
	int dist_ticks = dist * TICKS_PER_CM;
	ActionDrive leftDrive;
	leftDrive.enMotor = leftRear;
	ActionDrive rightDrive;
	rightDrive.enMotor = rightRear;
	at_init_drive(leftDrive, dist_ticks);
	at_init_drive(rightDrive, dist_ticks);
	while (!at_is_drive_done(leftDrive) && !at_is_drive_done(rightDrive))
	{
		int error = rightDrive.enDelta - leftDrive.enDelta;
		at_drive_update(leftDrive, error);
		//invert error for the right side, because error is right - left
		at_drive_update(rightDrive, -error);
	}
}

void dr_update()
{
	int forward = vexRT[Ch3];
	int rot = -vexRT[Ch1];
	dr_drive(forward, rot);
}
