#include "lib/util.h"

#define AUTO_DRIVE_ERROR_FAC 1
#define BRAKE_SLOPE 1

#define MOTOR_MAX 65

struct ActionDrive
{
	tMotor enMotor;
	int enLast;
	int enDelta;
	int begin;
	int target;
	int output;
};

void at_init_drive(ActionDrive* action, int encoderDist)
{
	action->begin = nMotorEncoder[action->enMotor];
	action->target = action->begin + encoderDist;
}

bool at_is_drive_done(ActionDrive* action)
{
	return abs(action->begin) < abs(action->target);
}

void at_drive_update(ActionDrive* action, int error)
{
	int encoder = nMotorEncoder[action->enMotor];
	int error_correct = error * AUTO_DRIVE_ERROR_FAC;
	int dist = action->target - action->begin;
	int pwr = BRAKE_SLOPE * dist;
	action->output = clamp(pwr, -MOTOR_MAX, MOTOR_MAX) + error_correct;
	action->enDelta = encoder - action->enLast;
	action->enLast = encoder;
}
