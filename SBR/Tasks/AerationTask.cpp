#include "../Task.h"
#include "../Device.h"

class AerationTask : public Task
{
public:
	AerationTask(Device * device)
	{
		this->device = device;
	}
private:
	Device * device = NULL;
protected:
	void onStart()
	{
		Serial.println("Start aeration");
		device->turnOnAerator();
	}

	void onFinish()
	{
		Serial.println("Finish aeration");
		device->turnOffAerator();
	}
};