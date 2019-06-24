#ifndef _IDEVICE_h
#define _IDEVICE_h

#include "IDeviceController.h"

class IDevice
{
public:
	IDevice() {}
	virtual ~IDevice() {}
	virtual void turnOnAerator() = 0;
	virtual void turnOffAerator() = 0;
	virtual void turnOnDecanter() = 0;
	virtual void turnOffDecanter() = 0;	
	virtual float getWaterTemperature() = 0;
	virtual bool getAeratorState() = 0;
	virtual bool getDecanterState() = 0;
	virtual void stop() = 0;
	virtual void registerController(IDeviceController *controller) = 0;
};


#endif

