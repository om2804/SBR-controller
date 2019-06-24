#include "stdafx.h"
#include "../SBR/Time.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{		
	TEST_CLASS(TimeTests)
	{
	public:		
		TEST_METHOD(CanAddMinutes)
		{
			Time* time = new Time((unsigned char)17, (unsigned char)56);
			time->addMinutes(5);
			Assert::AreEqual((unsigned char)1, time->minutes);
			Assert::AreEqual((unsigned char)18, time->hours);
			Assert::AreEqual((unsigned char)0, time->seconds);
			Assert::AreEqual((int)0, (int)time->millis);

			delete time;
		}

		TEST_METHOD(CanAddSeconds)
		{
			Time* time = new Time((unsigned char)17, (unsigned char)59);
			time->addSeconds(65);
			
			Assert::AreEqual((unsigned char)18, time->hours);
			Assert::AreEqual((unsigned char)0, time->minutes);
			Assert::AreEqual((unsigned char)5, time->seconds);
			Assert::AreEqual((int)0, (int)time->millis);

			delete time;
		}

		TEST_METHOD(CanAddMillis)
		{
			Time* time = new Time((unsigned char)17, (unsigned char)59);
			time->addMillis(65123);

			Assert::AreEqual((unsigned char)18, time->hours);
			Assert::AreEqual((unsigned char)0, time->minutes);
			Assert::AreEqual((unsigned char)5, time->seconds);
			Assert::AreEqual((int)123, (int)time->millis);

			delete time;
		}
	};
}