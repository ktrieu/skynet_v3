#define ARM_TOP 3750
#define ARM_BOTTOM 255
#define ARM_DEAD_ZONE 10

enum ArmCtrlState
{
	USER_CTRL,
	LOCK
};

struct ArmData
{
	int armTarget;
	PidCtrl armCtrl;
	ArmCtrlState ctrlState;
}


void arm_motor_test()
{
	motor[armLeft] = 75;
	wait1Msec(1000);
	motor[armLeft] = 0;
	motor[armRight] = 75;
	wait1Msec(1000);
	motor[armRight] = 0;
	wait1Msec(1000);
}

void arm_init(ArmData* data)
{
	data->armCtrl.gainP = 0.1;
	data->armCtrl.gainI = 0.005;
	data->armCtrl.gainD = 0.25;
	data->ctrlState = USER_CTRL;
	data->armCtrl.log = false;
	nMotorEncoder[armLeft] = 0;
}

void arm_set_motors(int val)
{
	motor[armLeft] = motor[armRight] = val;
}

void arm_update(ArmData* data)
{
	int axis_2 = vexRT[Ch2Xmtr2];
	if (abs(axis_2) > ARM_DEAD_ZONE)
	{
		arm_set_motors(axis_2);
		data->ctrlState = USER_CTRL;
	}
	else
	{
		if (data->ctrlState == USER_CTRL)
		{
			data->armCtrl.target = nMotorEncoder[armLeft];
		}
		data->ctrlState = LOCK;
	}
	if (data->ctrlState == LOCK)
	{
		pid_update(data->armCtrl, nMotorEncoder[armLeft]);
		arm_set_motors(0);
	}
}
