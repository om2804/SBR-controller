#ifndef _Time_h
#define _Time_h

class Time
{
public:
	Time(unsigned char hours, unsigned char minutes, unsigned char seconds);
	Time(unsigned char hours, unsigned char minutes);
	~Time();
	unsigned char hours = 0;
	unsigned char minutes = 0;
	unsigned char seconds = 0;
	unsigned short millis = 0;
	void addMillis(unsigned long mills);
	void addSeconds(unsigned long seconds);
	void addMinutes(unsigned long minutes);
	void addHours(unsigned long hours);
	int getTotalMinutes();
	void toString(char str[6]);
};
#endif
