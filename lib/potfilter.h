#define FILTER_ALPHA 1

struct PotFilter
{
	tSensors pot;
	int output;
	int outputLast;
};

void filter_init(PotFilter* filter, tSensors pot)
{
	filter->pot = pot;
}

void filter_update(PotFilter* filter)
{
	int new = FILTER_ALPHA * SensorValue[filter->pot] + (1 - FILTER_ALPHA) * filter->outputLast;
	filter->outputLast = filter->output;
	filter->output = new;
}
