#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <Settings.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#define DNS_PORT 53
#define DEFAULT_HTML "/index.html"

const IPAddress ACCESS_POINT_IP(192, 168, 0, 1);

class WiFiSetup
{
public:
	WiFiSetup(ESP8266WebServer &server1, DNSServer &dnsServer1, const char *apSsid, const uint8_t setupPin) : server(server1), dnsServer(dnsServer1), AP_SSID(apSsid), SETUP_PIN(setupPin) {}

	bool setup()
	{
		Settings::setup();
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

private:
	ESP8266WebServer &server;
	DNSServer &dnsServer;
	const char *AP_SSID;
	const uint8_t SETUP_PIN;

	void startAccessPoint()
	{
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(ACCESS_POINT_IP, ACCESS_POINT_IP, IPAddress(255, 255, 255, 0));
		WiFi.softAP(AP_SSID);
		dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
		dnsServer.start(DNS_PORT, "*", ACCESS_POINT_IP);
		startServer();
	}

	void connectToWifi()
	{
		Settings settings;
		WiFi.begin(settings.read("wifi-ssid", ""), settings.read("wifi-password", ""));
		startServer();
	}

	void startServer()
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
			settings.write(jsonDocument.as<JsonObjectConst>());
			server.send(200, "application/json", "{}");
		});
		server.on("/api/read-settings", HTTP_GET, [&]() { Settings::sendToServer(server); });
		server.begin();
	}

	static String getContentType(String filename)
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
};

#endif
