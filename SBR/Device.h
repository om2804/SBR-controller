#ifndef _DEVICE_h
#define _DEVICE_h

#include "IDevice.h"
#include "IDeviceController.h"
#include <DallasTemperature.h>  
#include <OneWire.h>


struct DeviceConfig
{
	unsigned char aeratorPin = 0;
	unsigned char decanterPin = 0;
	unsigned char waterTermometrPin = 0;
};


class Device : public IDevice
{
public:
	Device(DeviceConfig deviceConfig);
	~Device();

	void turnOnAerator();
	void turnOffAerator();
	void turnOnDecanter();
	void turnOffDecanter();
	float getWaterTemperature();
	bool getAeratorState();
	bool getDecanterState();
	void stop();
	void registerController(IDeviceController *controller);

private:
	DeviceConfig deviceConfig;
	bool aeratorState;
	bool decantState;
	bool drainingState;
	IDeviceController *controller = NULL;
	void fireEvent(DeviceEvent deviceEvent);
	OneWire *oneWire;
	DallasTemperature *DS18B20;
};

#endif