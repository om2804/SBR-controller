#ifndef _SCHEDULER_h
#define _SCHEDULER_h

#include "main.h"
#include "Time.h"
#include "ITimeProvider.h"
#include "Task.h"


extern "C" {
#include "cont.h"
}

extern void runCurrentTask();

class Scheduler
{
	struct Schedule
	{
		Time *startTime;
		Time *endTime;	
		Task * task;
		Schedule *next;
	};

public:
	Scheduler(ITimeProvider *timeProvider);
	~Scheduler();
	void schedule(Time *startTime, Time *endTime, Task * task);
	void loop();
	void setEnabled(bool enabled);
	bool getEnabled();
private:
	friend void runCurrentTask();
	ITimeProvider *timeProvider;
	Schedule *head;
	static Schedule *current;
	bool check(Time * startTime, Time * endTime, Time * now);
	bool enabled = true;
};
#endif
