#include "WiFiSetup.h"
#include <LittleFS.h>
#include <Settings.h>

#define DNS_PORT 53
#define DEFAULT_HTML "/index.html"

WiFiSetup::WiFiSetup(ESP8266WebServer &server1, DNSServer &dnsServer1, const char *apSsid, const uint8_t setupPin) : server(server1), dnsServer(dnsServer1), AP_SSID(apSsid), SETUP_PIN(setupPin) {}

bool WiFiSetup::setup()
{
	pinMode(SETUP_PIN, OUTPUT);

	if (digitalRead(SETUP_PIN))
	{
		startAccessPoint();
		return false;
	}
	else
	{
		connectToWifi();
		return true;
	}
}

void WiFiSetup::startAccessPoint()
{
	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(ACCESS_POINT_IP, ACCESS_POINT_IP, IPAddress(255, 255, 255, 0));
	WiFi.softAP(AP_SSID);
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(DNS_PORT, "*", ACCESS_POINT_IP);
	startServer();
}

void WiFiSetup::connectToWifi()
{
	Settings settings;
	WiFi.begin(settings.read("wiFiSsid", ""), settings.read("wiFiPassword", ""));
	startServer();
}

void WiFiSetup::startServer()
{
	server.onNotFound([&]() {
		String path = server.uri();

		if (path.equals("/") || !LittleFS.exists(path))
		{
			path = DEFAULT_HTML;
		}

		File file = LittleFS.open(path, "r");
		server.streamFile(file, getContentType(path));
		file.close();
	});
	server.on("/api/write-settings", HTTP_POST, [&]() {
		Settings settings;
		JsonDocument jsonDocument;
		deserializeJson(jsonDocument, server.arg("plain").c_str());
		settings.write(jsonDocument);
		Settings::sendToServer(server);
	});
	server.on("/api/read-settings", HTTP_GET, [&]() { Settings::sendToServer(server); });
	server.begin();
}

String WiFiSetup::getContentType(String filename)
{
	if (filename.endsWith(".html"))
	{
		return "text/html";
	}
	else if (filename.endsWith(".css"))
	{
		return "text/css";
	}
	else if (filename.endsWith(".js"))
	{
		return "application/javascript";
	}
	else if (filename.endsWith(".ico"))
	{
		return "image/x-icon";
	}
	else
	{
		return "text/plain";
	}
}
