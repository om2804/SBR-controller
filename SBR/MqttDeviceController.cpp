#include "MqttDeviceController.h"

MqttSbrController::MqttSbrController(IDevice *sbr1, IDevice *sbr2, Scheduler *scheduler, ITimeProvider *timeProvider)
{
	this->sbr1 = sbr1;
	this->sbr2 = sbr2;
	this->scheduler = scheduler;
	this->timeProvider = timeProvider;
	this->client = new PubSubClient(espClient);
	IPAddress ip;
	if (ip.fromString(MQTT_HOST))
	{
		this->client->setServer(ip, MQTT_PORT);
	}
	else
	{
		this->client->setServer(MQTT_HOST, MQTT_PORT);
	}	
	this->client->setCallback([this](char* topic, uint8_t* payload, unsigned int length) { this->callback(topic, payload, length); });
}


MqttSbrController::~MqttSbrController()
{
	delete client;
}

void MqttSbrController::reconnect()
{
	// Loop until we're reconnected
	while (!client->connected())
	{
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client->connect("SBR", MQTT_USER, MQTT_PASSWORD)) // ToDo
		{
			Serial.println("connected");
			client->subscribe("SBR/#");
		}
		else
		{
			Serial.print("failed, rc=");
			Serial.print(client->state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void MqttSbrController::publish(char * topic, char * message)
{
	if (!client->publish(topic, message))
	{
		Serial.println("not published");
	}
}


void MqttSbrController::loop()
{
	if (!client->connected())
	{
		reconnect();
	}

	unsigned long currentMills = millis();
	unsigned long duration = currentMills - this->prevMills;
	if (duration >= 30000)
	{
		this->prevMills = currentMills;
		this->sendState();
	}

	client->loop();
}


void MqttSbrController::switchToManual()
{
	scheduler->setEnabled(false);
	sbr1->stop();
}

void MqttSbrController::switchToAuto()
{
	sbr1->stop();
	scheduler->setEnabled(true);
}

void MqttSbrController::startAeration()
{
	if (this->scheduler->getEnabled()) return; // only manual mode
	this->sbr1->turnOnAerator();
}

void MqttSbrController::startDecant()
{
	if (this->scheduler->getEnabled()) return; // only manual mode
	this->sbr1->turnOnDecanter();
}

void MqttSbrController::startDecant2()
{
	if (this->scheduler->getEnabled()) return; // only manual mode
	this->sbr2->turnOnDecanter();
}


void MqttSbrController::stop()
{
	if (this->scheduler->getEnabled()) return; // only manual mode
	this->sbr1->stop();
	this->sbr2->stop();
}


void MqttSbrController::callback(char* topic, uint8_t* payload, unsigned int length) 
{
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("]: ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();


	if (!strcmp(topic, "SBR/1/decant"))
	{		
		bool decant = parsePayload("true", (char*)payload, length);
		if (decant)
		{
			this->startDecant();
		}
		else
		{
			this->stop();
		}		
	}
	else if (!strcmp(topic, "SBR/1/aeration"))
	{		
		bool aeration = parsePayload("true", (char*)payload, length);
		
		if (aeration)
		{
			this->startAeration();
		}
		else
		{
			this->stop();
		}		
	}
	else if (!strcmp(topic, "SBR/2/decant"))
	{
		bool decant = parsePayload("true", (char*)payload, length);
		if (decant)
		{
			this->startDecant2();
		}
		else
		{
			this->stop();
		}
	}
	else if (!strcmp(topic, "SBR/2/aeration"))
	{
		bool aeration = parsePayload("true", (char*)payload, length);

		if (aeration)
		{
			this->startAeration();
		}
		else
		{
			this->stop();
		}
	}
	else if (!strcmp(topic, "SBR/mode"))
	{
		bool autoMode = parsePayload("auto", (char*)payload, length);
		if (scheduler->getEnabled() != autoMode) {
			if (autoMode)
			{
				Serial.println("mode auto");
				this->switchToAuto();
			}
			else if (parsePayload("manual", (char*)payload, length))
			{
				Serial.println("mode manual");
				this->switchToManual();
			}
			this->sendState();
		}
	}		
}

void MqttSbrController::handleEvent(void *sender, DeviceEvent deviceEvent)
{
	char *topic;
	char *message;
	switch (deviceEvent)
	{
	case aeratorStateChanged:
		if (sbr1 == sender) message = (char*)(this->sbr1->getAeratorState() ? "true" : "false");
		else message = (char*)(this->sbr2->getAeratorState() ? "true" : "false");
		topic = (sbr1 == sender) ? (char*)"SBR/1/aeration" : (char*)"SBR/2/aeration";
		this->publish(topic, message);
		break;
	case decanterStateChanged:
		if (sbr1 == sender) message = (char*)(this->sbr1->getDecanterState() ? "true" : "false");
		else message = (char*)(this->sbr2->getDecanterState() ? "true" : "false");
		topic = (sbr1 == sender) ? (char*)"SBR/1/decant" : (char*)"SBR/2/decant";
		this->publish(topic, message);
		break;
	default:
		break;
	}
}

void MqttSbrController::sendState()
{	
	float temp = sbr1->getWaterTemperature();
	
	String tempStr = String(temp, DEC);
	this->publish("SBR/1/waterTemperature", (char*)tempStr.c_str());

	//this->publish("SBR/aeration", (char*)(this->device->getAerationState() ? "true" : "false"));
	//this->publish("SBR/decant", (char*)(this->device->getDecantState() ? "true" : "false"));
	//this->publish("SBR/draining", (char*)(this->device->getDrainingState() ? "true" : "false"));
	this->publish("SBR/mode", (char*)(this->scheduler->getEnabled() ? "auto" : "manual"));

	this->publish("SBR/heap", (char*)String(ESP.getFreeHeap(), DEC).c_str());

	Time *now = timeProvider->getCurrentTime();
	char str[5];
	now->toString(str);
	this->publish("SBR/time", str);
}

boolean MqttSbrController::parsePayload(char * str1, char * str2, int str2Len)
{
	const unsigned char *s1 = (const unsigned char *)str1;
	const unsigned char *s2 = (const unsigned char *)str2;
	unsigned char c1, c2;
	int len = str2Len;

	do {		
    if (len <= 0) return true;
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0') return (c1 - c2) == 0;
		len--;    
	} while (c1 == c2);

	return (c1 - c2) == 0;
}
