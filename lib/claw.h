#include "lib/potfilter.h"

#define CLAW_FAC 60
#define CLAW_REST 20

#define CLAW_DEAD_ZONE 10

//not recalibrated no use please
#define CLAW_ROT_TOP 3900
#define CLAW_ROT_BOTTOM 1500
#define CLAW_ROT_BACK 200

#define CLAW_STOP 1740

enum Direction
{
	UP,
	DOWN
};

enum ClawState
{
	OPEN,
	CLOSED
};

int clawPositions [3] = { CLAW_ROT_BACK, CLAW_ROT_BOTTOM, CLAW_ROT_TOP };

enum ClawRotState
{
	USER_CONTROL,
	LOCKED
};

struct ClawData
{
	bool lockClawPower;
	int clawPower;
	ClawState clawState;
	ClawRotState controlState;
	int clawRotPosIdx;
	PidCtrl clawCtrl;
	int lastEncoder;
};

int snap_claw_rot(int current, Direction dir)
{
	//find the nearest snap point going down
	if (dir == DOWN)
	{
		if (current > CLAW_ROT_TOP)
		{
			return CLAW_ROT_TOP;
		}
		else if (current > CLAW_ROT_BOTTOM)
		{
			return CLAW_ROT_BOTTOM;
		}
		else if (current > CLAW_ROT_BACK)
		{
			return CLAW_ROT_BACK;
		}
	}
	else if (dir == UP)
	{
		if (current < CLAW_ROT_BACK)
		{
			return CLAW_ROT_BACK;
		}
		else if (current < CLAW_ROT_BOTTOM)
		{
			return CLAW_ROT_BOTTOM;
		}
		else if (current < CLAW_ROT_TOP)
		{
			return CLAW_ROT_TOP;
		}
	}
	return current;
}

void claw_close(ClawData* data)
{
	data->clawPower = CLAW_FAC;
	data->lockClawPower = true;
}

void claw_open(ClawData* data)
{
	data->clawPower = -CLAW_FAC;
	data->lockClawPower = false;
}

void claw_init(ClawData* data)
{
	data->clawCtrl.gainP = 0.1;
	data->clawCtrl.gainI = 0;
	data->clawCtrl.gainD = 0.01;
	data->clawState = OPEN;
	data->controlState = USER_CONTROL;
	data->clawCtrl.log = true;
	data->clawRotPosIdx = 1;
	nMotorEncoder[clawRot] = 0;
	nMotorEncoder[claw] = 0;
}

void adjust_drive(ClawData* data)
{
	int current = nMotorEncoder[clawRot];
	int delta = current - data->lastEncoder;
	float degrees = (float)delta * ((float)CLAW_STOP / (float)180);
	float dist = 1 - cosDegrees(degrees);
	//we move backwards if we're in the lower half of rotation
	if (current < CLAW_STOP / 2)
	{
		dist = -dist;
	}
	writeDebugStreamLine("%d", degrees);
	if (abs(dist) > 0.1)
	{
		//dr_drive(dist * 50, 0);
	}
	data->lastEncoder = current;
}

void claw_update_rot(ClawData* data)
{
	adjust_drive(data);
	//if (joy_is_clicked(Btn5UXmtr2) == 1)
	//{
	//	//close the claw when moving the arm to avoid jamming
	//	claw_close(data);
	//	data->clawCtrl.target = snap_claw_rot(data->clawCtrl.target, UP);
	//	data->controlState = LOCK;
	//}
	//if (joy_is_clicked(Btn5DXmtr2) == 1)
	//{
	//	//ditto
	//	claw_close(data);
	//	data->clawCtrl.target = snap_claw_rot(data->clawCtrl.target, DOWN);
	//	data->controlState = LOCK;
	//}
	//manual control
	int axis_3_par = vexRT[Ch3Xmtr2];
	if (abs(axis_3_par) > CLAW_DEAD_ZONE)
	{
		claw_close(data);
		//if we're moving down or we're not at the stop
		if (axis_3_par < 0 || nMotorEncoder[clawRot] <= CLAW_STOP)
		{
			motor[clawRot] = axis_3_par;
			data->controlState = USER_CONTROL;
		}
		else
		{
			//if we are stopped, lock at the stop level
			data->clawCtrl.target = CLAW_STOP;
			data->controlState = LOCK;
		}
	}
	else
	{
		if (data->controlState == USER_CONTROL)
		{
			data->clawCtrl.target = nMotorEncoder[clawRot];
		}
		data->controlState = LOCK;
	}
	if (data->controlState == LOCK)
	{
		pid_update(data->clawCtrl, nMotorEncoder[clawRot]);
		motor[clawRot] = data->clawCtrl.output;
	}
}

void claw_update(ClawData* data)
{
	claw_update_rot(data);
	if (vexRT[Btn6UXmtr2] == 1)
	{
		claw_close(data);
	}
	else if (vexRT[Btn6DXmtr2] == 1)
	{
		claw_open(data);
	}
	else
	{
		if (data->lockClawPower)
		{
			data->clawPower = CLAW_REST;
		}
		else
		{
			data->clawPower = 0;
		}
	}
	motor[claw] = data->clawPower;
}
