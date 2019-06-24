#include "Task.h"
#include "main.h"

Task::Task()
{
	cont_init(&context);
}


Task::~Task()
{
}

void Task::loopWrapper()
{
	loop();
}

bool Task::tryStart()
{
	if (!isStarted)
	{
		isStarted = true;
		onStart();
		return true;
	}
	return false;
}

bool Task::tryStop()
{
	if (isStarted)
	{
		isStarted = false;
		onFinish();
		return true;
	}
	return false;
}

void Task::delay(unsigned long ms)
{
	delayMs = ms;
	delayStart = millis();

	cont_yield(&context);
}

bool Task::isAwait()
{
	if (delayMs == 0) return false;
	unsigned long now = millis();
	if ((now - delayStart) >= delayMs)
	{
		delayMs = 0;
		return false;
	}

	return true;
}
