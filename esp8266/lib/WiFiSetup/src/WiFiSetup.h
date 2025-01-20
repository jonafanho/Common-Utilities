#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>

const IPAddress ACCESS_POINT_IP(192, 168, 0, 1);

class WiFiSetup
{
public:
	WiFiSetup(ESP8266WebServer &server1, DNSServer &dnsServer1, const char *apSsid, const uint8_t setupPin);
	bool setup();

private:
	ESP8266WebServer &server;
	DNSServer &dnsServer;
	const char *AP_SSID;
	const uint8_t SETUP_PIN;

	void startAccessPoint();
	void connectToWifi();
	void startServer();
	static String getContentType(String filename);
};

#endif
