#include "MqttDeviceController.h"
#include "Device.h"
#include "TimeProvider.h"
#include "scheduler.h"
#include "Task.h"
#include "OTAUpdate.h"
#include "Network.h"
#include "Tasks/DenitrificationTask.cpp"
#include "Tasks/SettleTask.cpp"
#include "Tasks/DecantTask.cpp"
#include "Tasks/AerationTask.cpp"


Network *network;
Scheduler *scheduler;
TimeProvider *timeProvider;
Device *sbr1;
Device *sbr2;
IDeviceController *controller;
OTAUpdate *otaUpdate;


// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(115200);		
  
	network = new Network();
	otaUpdate = new OTAUpdate();
	timeProvider = new TimeProvider(network);
	timeProvider->utcOffset = UTC_OFFSET;
	scheduler = new Scheduler(timeProvider);

	DeviceConfig sbr1Config;
	sbr1Config.aeratorPin = AERATOR1_PIN;
	sbr1Config.decanterPin = DECANTER1_PIN;
	sbr1Config.waterTermometrPin = WATER_TERMOMETR1_PIN;
	sbr1 = new Device(sbr1Config);

	DeviceConfig sbr2Config;
	sbr2Config.aeratorPin = AERATOR2_PIN;
	sbr2Config.decanterPin = DECANTER2_PIN;
	sbr2 = new Device(sbr2Config);

	controller = new MqttSbrController(sbr1, sbr2, scheduler, timeProvider);
	sbr1->registerController(controller);
	sbr2->registerController(controller);
	SettleTask * settleTask = new SettleTask(sbr1);
	DecantTask * decantTask = new DecantTask(sbr1);
	AerationTask * aerationTask = new AerationTask(sbr1);
	DenitrificationTask * denitrificationTask = new DenitrificationTask(sbr1);
	DecantTask *decantTask2 = new DecantTask(sbr2);
	
	// Schedule for SBR 1
	// cycle 1
	scheduler->schedule(new Time(6, 00), new Time(8, 00), aerationTask);
	scheduler->schedule(new Time(8, 00), new Time(10, 00), denitrificationTask);
	scheduler->schedule(new Time(10, 00), new Time(12, 00), aerationTask);
	scheduler->schedule(new Time(12, 00), new Time(14, 00), denitrificationTask);
	scheduler->schedule(new Time(14, 00), new Time(16, 00), aerationTask);
	scheduler->schedule(new Time(16, 00), new Time(17, 00), denitrificationTask);
	scheduler->schedule(new Time(17, 00), new Time(17, 50), settleTask);	
	scheduler->schedule(new Time(17, 50), new Time(18, 00), decantTask);
	// cycle 2
	scheduler->schedule(new Time(18, 00), new Time(20, 00), aerationTask);
	scheduler->schedule(new Time(20, 00), new Time(22, 00), denitrificationTask);
	scheduler->schedule(new Time(22, 00), new Time(0, 00), aerationTask);
	scheduler->schedule(new Time(0, 00), new Time(2, 00), denitrificationTask);
	scheduler->schedule(new Time(2, 00), new Time(4, 00), aerationTask);
	scheduler->schedule(new Time(4, 00), new Time(5, 00), denitrificationTask);
	scheduler->schedule(new Time(5, 00), new Time(5, 50), settleTask);	
	scheduler->schedule(new Time(5, 50), new Time(6, 00), decantTask);

	// Schedule for SBR 2
	// cycle 1
	scheduler->schedule(new Time(17, 30), new Time(17, 45), decantTask2);
	// cycle 2
	scheduler->schedule(new Time(5, 30), new Time(5, 45), decantTask2);
}

// Add the main program code into the continuous loop() function
void loop()
{
	otaUpdate->loop();
	controller->loop();   
	scheduler->loop();	

	yield();
}
