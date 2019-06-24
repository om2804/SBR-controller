#include "scheduler.h"

Scheduler::Schedule *Scheduler::current = 0;

Scheduler::Scheduler(ITimeProvider *timeProvider)
{
	this->timeProvider = timeProvider;
	this->head = 0;
}

Scheduler::~Scheduler()
{
	// delete schedule
	Schedule *current = head;
	while (current != 0)
	{
		current->task->tryStop();
		Schedule *next = current->next;
		if (current != 0) delete current;
		current = next;
		
		yield();
	}
}

void Scheduler::schedule(Time * startTime, Time * endTime, Task * task)
{
	Schedule *current = new Schedule();
	current->startTime = startTime;
	current->endTime = endTime;
	current->task = task;
	current->next = head;

	head = current;
}

void Scheduler::loop()
{
	if (!enabled) return;

	Time *now = timeProvider->getCurrentTime();    

	current = head;
	while (current != 0) 
	{
		if (check(current->startTime, current->endTime, now))
		{
			if (current->task->tryStart())
			{

			}
			if (!current->task->isAwait())
			{
				cont_run(&current->task->context, runCurrentTask);
			}
		}
		else
		{
			if (current->endTime->hours == now->hours && current->endTime->minutes == now->minutes)
			{
			  current->task->tryStop();
			}
		}		

		current = current->next;
		yield();
	}
}

void Scheduler::setEnabled(bool enabled)
{
	this->enabled = enabled;
	if (!enabled)
	{
		Schedule *current = head;
		while (current != 0)
		{			
			current->task->tryStop();
			current = current->next;
			yield();
		}
	}
}

bool Scheduler::getEnabled()
{
	return this->enabled;
}

void runCurrentTask() {
	Scheduler::current->task->loopWrapper();
}

bool Scheduler::check(Time *startTime, Time *endTime, Time * now)
{	
	int start = startTime->getTotalMinutes();
	int end = endTime->getTotalMinutes();
	int currentTime = now->getTotalMinutes();
		

	if (start < end)
	{
		return currentTime >= start && currentTime < end;
	}
	else if (start > end)
	{     
		return (currentTime >= start && currentTime <= 1440) || (currentTime >= 0 && currentTime < end);
	}
	else
	{
		return true;
	}	
}
