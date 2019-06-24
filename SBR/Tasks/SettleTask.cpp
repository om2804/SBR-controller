#include "../Task.h"
#include "../Device.h"

class SettleTask : public Task
{
public:
	SettleTask(Device * device)
	{
		this->device = device;
	}
private:
	Device * device = NULL;
protected:
	void onStart()
	{
		Serial.println("Start settle");
		device->stop();
	}
};