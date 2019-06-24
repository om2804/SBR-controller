#pragma once

#include "stdafx.h"
#include "../SBR/ITimeProvider.h"

class FakeTimeProvider : public ITimeProvider
{
public:
	Time *getCurrentTime()
	{
		return this->time;
	}

	void setTime(unsigned char hours, unsigned char minutes, unsigned char seconds)
	{
		if (time != 0) delete time;
		time = new Time(hours, minutes, seconds);
	}
private:
	Time *time = 0;
};