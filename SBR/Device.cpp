#include "Device.h"


Device::Device(DeviceConfig deviceConfig)
{
	this->deviceConfig = deviceConfig;
	if (this->deviceConfig.aeratorPin) pinMode(this->deviceConfig.aeratorPin, OUTPUT);
	if (this->deviceConfig.decanterPin) pinMode(this->deviceConfig.decanterPin, OUTPUT);
	
	oneWire = new OneWire(this->deviceConfig.waterTermometrPin);
	DS18B20 = new DallasTemperature(oneWire);

	this->stop();
}

Device::~Device()
{
	delete this->DS18B20;
	delete this->oneWire;
}

void Device::turnOnAerator()
{
	if (aeratorState || !this->deviceConfig.aeratorPin) return;
	digitalWrite(this->deviceConfig.aeratorPin, HIGH);
	this->aeratorState = true;
	this->fireEvent(aeratorStateChanged);
  Serial.println("Aerator - on");
}

void Device::turnOffAerator()
{
	if (!aeratorState || !this->deviceConfig.aeratorPin) return;
	digitalWrite(this->deviceConfig.aeratorPin, LOW);
	this->aeratorState = false;
	this->fireEvent(aeratorStateChanged);
  Serial.println("Aerator - off");
}

void Device::turnOnDecanter()
{
	if (decantState || !this->deviceConfig.decanterPin) return;
	digitalWrite(this->deviceConfig.decanterPin, HIGH);
	this->decantState = true;
	this->fireEvent(decanterStateChanged);
  Serial.println("Decanter - on");
}

void Device::turnOffDecanter()
{
	if (!decantState || !this->deviceConfig.decanterPin) return;
	digitalWrite(this->deviceConfig.decanterPin, LOW);
	this->decantState = false;
	this->fireEvent(decanterStateChanged);
  Serial.println("Decanter - off");
}

float Device::getWaterTemperature()
{
	DS18B20->requestTemperatures();
	return DS18B20->getTempCByIndex(0);
}

bool Device::getAeratorState()
{
	return this->aeratorState;
}

bool Device::getDecanterState()
{
	return this->decantState;
}

void Device::stop()
{
	this->turnOffAerator();
	this->turnOffDecanter();
}

void Device::registerController(IDeviceController *controller)
{
	this->controller = controller;
}

void Device::fireEvent(DeviceEvent deviceEvent)
{
	if (this->controller != 0) this->controller->handleEvent(this, deviceEvent);
}


