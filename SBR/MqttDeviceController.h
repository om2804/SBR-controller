#ifndef _MqttDeviceController_h
#define _MqttDeviceController_h

#include "main.h"
#include "IDeviceController.h"
#include "IDevice.h"
#include "scheduler.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MqttSbrController : public IDeviceController
{
public:
	MqttSbrController(IDevice * sbr1, IDevice * sbr2, Scheduler *scheduler, ITimeProvider *timeProvider);
	~MqttSbrController();
	void reconnect();
	void publish(char * topic, char * message);
	void loop();

	void switchToManual();
	void switchToAuto();
	void startAeration();
	void startDecant();
	void startDecant2();
	void stop();
	void handleEvent(void *sender, DeviceEvent deviceEvent);

private:
	WiFiClient espClient;
	PubSubClient *client;
	IDevice *sbr1;
	IDevice *sbr2;
	Scheduler *scheduler;
	ITimeProvider *timeProvider;
	void callback(char * topic, uint8_t * payload, unsigned int length);
	void sendState();
	boolean parsePayload(char* str1, char* str2, int str2Len);
	unsigned long prevMills = 0;
};

#endif

