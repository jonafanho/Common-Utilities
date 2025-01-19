#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <LittleFS.h>

#define DNS_PORT 53
#define DEFAULT_HTML "/index.html"
#define WIFI_FILE "/wifi.txt"

enum WiFiStatus
{
	WIFI_STATUS_AP = 0,
	WIFI_STATUS_CONNECTING = 1,
	WIFI_STATUS_CONNECTED = 2,
	WIFI_STATUS_FAILED = 3
};

const IPAddress ACCESS_POINT_IP(192, 168, 0, 1);

class WiFiSetup
{
public:
	WiFiSetup(ESP8266WebServer &server1, DNSServer &dnsServer1, const char *apSsid, const uint8_t setupPin) : server(server1), dnsServer(dnsServer1), AP_SSID(apSsid), SETUP_PIN(setupPin) {}

	bool setup()
	{
		LittleFS.begin();
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

		server.onNotFound([&]() { onRequest(); });
		server.on("/api/save", HTTP_GET, [&]() {
			File wifiFile = LittleFS.open(WIFI_FILE, "w");
			if (wifiFile)
			{
				wifiFile.print(server.arg("ssid"));
				wifiFile.print('\n');
				wifiFile.print(server.arg("password"));
				wifiFile.print('\n');
				wifiFile.close();
				server.send(200, "application/json", "{\"success\":true}");
			}
			else
			{
				server.send(200, "application/json", "{\"success\":false}");
			}
		});
		server.on("/api/status", HTTP_GET, [&]() {
			char ssid[256], password[256];
			if (readWifiFile(ssid, password))
			{
				char response[536];
				sprintf(response, "{\"ssid\":\"%s\",\"password\":\"%s\"}", ssid, password);
				server.send(200, "application/json", response);
			}
			else
			{
				server.send(200, "application/json", "{\"ssid\":\"\",\"password\":\"\"}");
			}
		});
		server.begin();
	}

	void connectToWifi()
	{
		char ssid[256], password[256];
		if (readWifiFile(ssid, password))
		{
			WiFi.begin(ssid, password);
		}

		server.onNotFound([&]() { onRequest(); });
		server.on("/api/status", HTTP_GET, [&]() { server.send(200, "application/json", "{}"); });
		server.begin();
	}

	void onRequest()
	{
		String path = server.uri();
		File file;

		if (path.equals("/") || !LittleFS.exists(path))
		{
			path = DEFAULT_HTML;
		}

		file = LittleFS.open(path, "r");
		server.streamFile(file, getContentType(path));
		file.close();
	}

	bool readWifiFile(char *ssid, char *password)
	{
		File wifiFile = LittleFS.open(WIFI_FILE, "r");
		if (wifiFile)
		{
			const uint8_t ssidLength = wifiFile.readBytesUntil('\n', ssid, 256);
			ssid[ssidLength] = 0;

			const uint8_t passwordLength = wifiFile.readBytesUntil('\n', password, 256);
			password[passwordLength] = 0;

			wifiFile.close();
			return true;
		}
		else
		{
			return false;
		}
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
