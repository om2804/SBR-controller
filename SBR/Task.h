#ifndef _Task_h
#define _Task_h


extern "C" {
#include "cont.h"
}

class Task
{
public:
	Task();
	~Task();
	void loopWrapper();
	bool tryStart();
	bool tryStop();
protected:
	void delay(unsigned long ms);
	virtual void loop() {}	
	virtual bool isAwait();
	virtual void onStart() {}
	virtual void onFinish() {}	
	bool isStarted = false;
private:
	friend class Scheduler;
	unsigned long delayStart = 0;
	unsigned long delayMs = 0;
	cont_t context;	
};

#endif
