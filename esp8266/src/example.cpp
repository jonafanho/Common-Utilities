#include <CustomScheduler.h>
#include <Settings.h>
#include <WiFiSetup.h>

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <WiFiClientSecureBearSSL.h>

#define PIN_BUTTON D0
#define ACCESS_POINT_SSID "Test"

ESP8266WebServer server(80);
DNSServer dnsServer;
WiFiSetup wiFiSetup(server, dnsServer, ACCESS_POINT_SSID, PIN_BUTTON);
CustomScheduler customScheduler;

void setup()
{
	Serial.begin(9600);
	Serial.println();
	Serial.println("Starting...");
	LittleFS.begin();

	bool isNormalConnection = wiFiSetup.setup();
	server.on("/api/clear-settings", HTTP_GET, [&]() {
		Settings::clear();
		server.send(200, "application/json", "{}");
	});
	server.on("/api/read-settings-number", HTTP_GET, [&]() {
		const char *key = server.arg("field").c_str();
		Settings settings;
		char response[256];
		sprintf(response, "{\"%s\":%f}", key, settings.read(key, 0.0));
		server.send(200, "application/json", response);
	});
	server.on("/api/read-settings-string", HTTP_GET, [&]() {
		const char *key = server.arg("field").c_str();
		Settings settings;
		char response[256];
		sprintf(response, "{\"%s\":\"%s\"}", key, settings.read(key, ""));
		server.send(200, "application/json", response);
	});
	server.on("/api/add-schedule", HTTP_GET, [&]() {
		customScheduler.add(server.arg("hour").toInt(), server.arg("minute").toInt(), server.arg("second").toInt(), server.arg("channel").toInt());
		Settings::sendToServer(server);
	});
	server.on("/api/remove-schedule", HTTP_GET, [&]() {
		customScheduler.remove(server.arg("hour").toInt(), server.arg("minute").toInt(), server.arg("second").toInt(), server.arg("channel").toInt());
		Settings::sendToServer(server);
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

	customScheduler.setup();
}

void loop()
{
	dnsServer.processNextRequest();
	server.handleClient();

	uint8_t customSchedulerChannel = customScheduler.tick();
	if (customSchedulerChannel > 0)
	{
		Serial.println(customSchedulerChannel);
	}
}
