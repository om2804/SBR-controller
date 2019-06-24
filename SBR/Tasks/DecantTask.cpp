#include "../Task.h"
#include "../Device.h"

class DecantTask : public Task
{
public:
	DecantTask(Device * device)
	{
		this->device = device;
	}
private:
	Device * device = NULL;
protected:
	void onStart()
	{
		Serial.println("Start effluent");
		device->turnOnDecanter();
	}

	void onFinish()
	{
		Serial.println("Stop effluent");
		device->turnOffDecanter();
	}
};