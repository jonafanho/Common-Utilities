#include <WiFiSetup.h>

#define PIN_BUTTON D0

#define ACCESS_POINT_SSID "Test"
#define SETTINGS_FILE "/settings.js"

ESP8266WebServer server(80);
DNSServer dnsServer;
WiFiSetup wiFiSetup(server, dnsServer, ACCESS_POINT_SSID, PIN_BUTTON);

void setup()
{
	Serial.begin(9600);
	Serial.println();
	Serial.println("Starting...");
	wiFiSetup.setup([]() {}, [](WiFiStatus wiFiStatus, char *subtitle) {
		Serial.println(subtitle);
		if (wiFiStatus == WIFI_STATUS_FAILED)
		{
			Serial.println("Connection failed!");
		}
	});
}

void loop()
{
	dnsServer.processNextRequest();
	server.handleClient();
}
