#include <WiFiSetup.h>
#include <HttpRequest.h>

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

	bool isNormalConnection = wiFiSetup.setup();
	server.on("/api/time", HTTP_GET, [&]() {
		const char *url = "https://timeapi.io/api/time/current/zone?timeZone=Asia/Hong_Kong";
		JsonDocument jsonDocument;
		Serial.println(httpGet(url, &jsonDocument));
		char response[256];
		sprintf(response, "{\"time\":\"%s\"}", jsonDocument["dateTime"].as<const char*>());
		server.send(200, "application/json", response);
	});

	if (isNormalConnection)
	{
		while (WiFi.status() != WL_CONNECTED)
		{
			delay(500);
		}
		Serial.print("Connected to WiFi: ");
		Serial.println(WiFi.localIP().toString());
	}
	else
	{
		Serial.print("Access point mode: ");
		Serial.println(ACCESS_POINT_IP.toString());
	}
}

void loop()
{
	dnsServer.processNextRequest();
	server.handleClient();
}
