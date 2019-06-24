#pragma once

#include "stdafx.h"
#include "../SBR/Task.h"


class SimpleTask : public Task
{
public:
	SimpleTask()
	{

	}

	bool _isStarted() {
		return isStarted;
	}
private:

protected:

};