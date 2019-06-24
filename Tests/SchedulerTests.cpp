#include "stdafx.h"

#include "cont.h"
#include "arduino.h"

#include "../SBR/Task.cpp"
#include "../SBR/scheduler.cpp"
#include "FakeTimeProvider.cpp"
#include "SimpleTask.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Tests
{
	TEST_CLASS(SchedulerTests)
	{
	public:
		TEST_METHOD(TestScheduler) // ToDo: split it
		{
			FakeTimeProvider *timeProvider = new FakeTimeProvider();
			timeProvider->setTime(1, 30, 0);
			SimpleTask *task1 = new SimpleTask();
			SimpleTask *task2 = new SimpleTask();
			SimpleTask *task3 = new SimpleTask();
			SimpleTask *task4 = new SimpleTask();
			Scheduler *scheduler = new Scheduler(timeProvider);
			scheduler->schedule(new Time(1, 0, 0), new Time(2, 00, 0), task1);
			scheduler->schedule(new Time(22, 0, 0), new Time(2, 00, 0), task2);
			scheduler->schedule(new Time(1, 0, 0), new Time(1, 30, 0), task3);
			scheduler->schedule(new Time(1, 30, 0), new Time(2, 00, 0), task4);
			scheduler->loop();

			Assert::AreEqual(true, task1->_isStarted());
			Assert::AreEqual(true, task2->_isStarted());
			Assert::AreEqual(false, task3->_isStarted());
			Assert::AreEqual(true, task4->_isStarted());

			delete scheduler;
			delete task1;
			delete task2;
			delete task3;
			delete task4;
			delete timeProvider;
		}
	};
}