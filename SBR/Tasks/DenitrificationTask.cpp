#include "../Task.h"
#include "../Device.h"

class DenitrificationTask : public Task
{
public:
	DenitrificationTask(Device * device)
	{
		this->device = device;
	}
private:
	Device * device = NULL;
protected:
	void loop()
	{
		Serial.println("Denitrification");
		// mixing
		Serial.println("Denitrification. Mixing");
		device->turnOnAerator();
		delay(10000); // 10 sek
		// inactivity
		Serial.println("Denitrification. Inactivity");
		device->turnOffAerator();
		delay(300000); //5 min
	}

	void onStart()
	{
		Serial.println("Start denitrification");
	}
};
