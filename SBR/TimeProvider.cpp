#include "TimeProvider.h"

TimeProvider::TimeProvider(Network *network)
{ 
	udp = network->createUdp();
	Serial.println("Starting UDP");
	udp->begin(localPort);
	Serial.print("Local port: ");
	Serial.println(udp->localPort());
}

TimeProvider::~TimeProvider()
{
  if (currentTime != 0) delete currentTime;
  if (udp != 0) delete udp;
}

Time* TimeProvider::getCurrentTime()
{	
	unsigned long currentMills = millis();
	unsigned long duration = currentMills - prevMills;
	prevMills = currentMills;
  
	if (duration < 0)
	{    
		return this->currentTime;
	}

	this->lastUpdate += duration;

	if (currentTime == 0 || this->lastUpdate > 300000)
	{   
		this->lastUpdate = 0;
		//get a random server from the pool
		WiFi.hostByName(ntpServerName, timeServerIP); 
  
		sendNTPpacket(timeServerIP); // send an NTP packet to a time server
		// wait to see if a reply is available
		delay(1000);
    
		int cb = udp->parsePacket();
		if (!cb) {
			Serial.println("no packet yet");      
		}
		else 
		{
			Serial.print("packet received, length=");
			Serial.println(cb);
			// We've received a packet, read the data from it
			udp->read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  
			//the timestamp starts at byte 40 of the received packet and is four bytes,
			// or two words, long. First, esxtract the two words:
  
			unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			// combine the four bytes (two words) into a long integer
			// this is NTP time (seconds since Jan 1 1900):
			unsigned long secsSince1900 = highWord << 16 | lowWord;
  
			// now convert NTP time into everyday time:
			// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
			const unsigned long seventyYears = 2208988800UL;
			// subtract seventy years:
			unsigned long epoch = secsSince1900 - seventyYears;
      
			int hours = (epoch  % 86400L) / 3600;
			int minutes = (epoch  % 3600) / 60;
			int seconds = (epoch % 60); 
     
			if (currentTime != 0) delete this->currentTime;
			this->currentTime = new Time(hours, minutes, seconds);
			this->currentTime->addHours(utcOffset);
  
			return currentTime;
		}
	}
	currentTime->addMillis(duration);   
 
	return currentTime;
}

void TimeProvider::sendNTPpacket(IPAddress& address)
{
	Serial.println("sending NTP packet...");
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp->beginPacket(address, 123); //NTP requests are to port 123
	udp->write(packetBuffer, NTP_PACKET_SIZE);
	udp->endPacket();
}
