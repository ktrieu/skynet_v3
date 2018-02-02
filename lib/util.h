int clamp(int val, int lo, int hi)
{
	if (val > hi)
	{
		return hi;
	}
	else if (val < lo)
	{
		return lo;
	}
	else
	{
		return val;
	}
}

int clamp_motor(int val)
{
	return clamp(val, -127, 127);
}
