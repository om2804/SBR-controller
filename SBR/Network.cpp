#include "Network.h"

char ssid[] = WIFI_SSID;  //  your network SSID (name)
char pass[] = WIFI_PASSWORD;   // your network password

Network::Network()
{
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, pass);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) 
	{
		Serial.println("Connection Failed! Rebooting...");
		ESP.restart();
	}
	Serial.println("");

	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

Network::~Network()
{
}

WiFiUDP* Network::createUdp()
{
	return new WiFiUDP();
}
