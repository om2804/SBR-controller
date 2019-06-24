#ifndef _TIMEPROVIDER_h
#define _TIMEPROVIDER_h

#include "ITimeProvider.h"
#include "Network.h"
#include "main.h"


#define NTP_PACKET_SIZE  48 // NTP time stamp is in the first 48 bytes of the message

class TimeProvider : public ITimeProvider
{
public:	
	TimeProvider(Network *network);
	~TimeProvider();
	Time* getCurrentTime();
	char utcOffset = 0;
 private:
	unsigned long lastUpdate = 0;
	unsigned long prevMills = 0;
	Time *currentTime = 0;
	unsigned int localPort = NTP_PORT;
	IPAddress timeServerIP;
	const char* ntpServerName = NTP_SERVER;
	WiFiUDP *udp = 0;
	byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming and outgoing packets
	void sendNTPpacket(IPAddress& address);
};
#endif
