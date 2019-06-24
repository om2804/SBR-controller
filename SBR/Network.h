#ifndef _NETWORK_h
#define _NETWORK_h

#include "main.h"
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

class Network
{
public:
	Network();
	~Network();
	WiFiUDP * createUdp();
};

#endif

