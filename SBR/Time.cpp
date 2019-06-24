#include "Time.h"

Time::Time(unsigned char hours, unsigned char minutes, unsigned char seconds)
{
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;
}

Time::Time(unsigned char hours, unsigned char minutes)
{
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = 0;
}

Time::~Time()
{
}

void Time::addMillis(unsigned long millis)
{
  if (millis == 0) return;

  unsigned long newMillis = this->millis + millis;
  this->millis = (unsigned short)(newMillis % 1000L);
  this->addSeconds(newMillis / 1000L);
}

void Time::addSeconds(unsigned long seconds)
{
  if (seconds == 0) return;

  unsigned char newSeconds = this->seconds + seconds;
  this->seconds = newSeconds % 60;
  this->addMinutes(newSeconds / 60L);
}

void Time::addMinutes(unsigned long minutes)
{
  if (minutes == 0) return;
   
   unsigned char newMinutes = this->minutes + minutes;
   this->addHours(newMinutes / 60L);
   this->minutes = newMinutes % 60;
}

void Time::addHours(unsigned long hours)
{
  if (hours == 0) return;
  if (this->hours + hours >= 24)
  {
    this->hours = (this->hours+hours) % 24;
  }
  else
  {
    this->hours += (unsigned char)hours;
  }
}

int Time::getTotalMinutes()
{
	return hours * 60 + minutes;
}

void Time::toString(char str[6])
{
      str[0] = (hours / 10) + '0';
      str[1] = (hours % 10) + '0';
      str[2] = ':';
      str[3] = (minutes / 10) + '0';
      str[4] = (minutes % 10) + '0';
      str[5] = 0;
}
