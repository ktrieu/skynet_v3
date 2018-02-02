#define MOGO_EPSILON 25
#define MOGO_BOTTOM 1100
#define MOGO_TOP 0

#define MOGO_PWR 1

enum MogoState
{
	GROUND,
	LIFTED
};

struct MogoData
{
	MogoState state;
	tMotor mogoMotor;
	tSensors mogoPot;
	int ticksStuck;
	int lastEncoder;
};

void mogo_init(MogoData* data, tMotor mot)
{
	data->mogoMotor = mot;
	data->state = LIFTED;
	nMotorEncoder[data->mogoMotor] = 0;
}

void mogo_update(MogoData* data)
{
	int output = 0;
	int current = nMotorEncoder[data->mogoMotor];
	if (joy_is_clicked(Btn7UXmtr2) == 1)
	{
		data->state = LIFTED;
		data->ticksStuck = 0;
	}
	if (joy_is_clicked(Btn7DXmtr2) == 1)
	{
		data->state = GROUND;
		data->ticksStuck = 0;
	}
	if (data->state == LIFTED)
	{
		 output = clamp_motor(MOGO_PWR * (MOGO_TOP - current));
	}
	if (data->state == GROUND)
	{
		output = clamp_motor(MOGO_PWR * (MOGO_BOTTOM - current));
	}
	if (abs(current - data->lastEncoder) < MOGO_EPSILON)
	{
		//we haven't moved that much since the last frame
		data->ticksStuck++;
	}
	else
	{
		data->ticksStuck = 0;
	}
	if (data->ticksStuck > 20)
	{
		//we've been stuck for more than ten frames give up
		output = 0;
	}
	motor[data->mogoMotor] = output;
	data->lastEncoder = current;
}
