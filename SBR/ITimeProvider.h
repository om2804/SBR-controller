#ifndef _ITimeProvider_h
#define _ITimeProvider_h

#include "Time.h"
class ITimeProvider
{
public:
	ITimeProvider() {}
	virtual ~ITimeProvider() {}
	virtual Time* getCurrentTime() = 0;
};
#endif




