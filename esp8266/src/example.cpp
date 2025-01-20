#include <WiFiSetup.h>
#include <HttpRequest.h>

#define PIN_BUTTON D0
#define ACCESS_POINT_SSID "Test"

ESP8266WebServer server(80);
DNSServer dnsServer;
WiFiSetup wiFiSetup(server, dnsServer, ACCESS_POINT_SSID, PIN_BUTTON);

void setup()
{
	Serial.begin(9600);
	Serial.println();
	Serial.println("Starting...");

	bool isNormalConnection = wiFiSetup.setup();
	server.on("/api/clear-settings", HTTP_GET, [&]() {
		Settings::clear();
		server.send(200, "application/json", "{}");
	});
	server.on("/api/time", HTTP_GET, [&]() {
		const char *url = "https://timeapi.io/api/time/current/zone?timeZone=Asia/Hong_Kong";
		JsonDocument jsonDocument;
		httpGet(url, &jsonDocument);
		char response[256];
		sprintf(response, "{\"time\":\"%s\"}", jsonDocument["dateTime"].as<const char *>());
		server.send(200, "application/json", response);
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
