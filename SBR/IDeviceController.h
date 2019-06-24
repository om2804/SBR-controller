#ifndef _IDEVICECONTROLLER_h
#define _IDEVICECONTROLLER_h

#include "DeviceEvent.h"

class IDeviceController
{
public:
	IDeviceController() {}
	virtual ~IDeviceController() {}
	virtual void handleEvent(void *sender, DeviceEvent deviceEvent) = 0;
	virtual void loop() = 0;
};

#endif

