#ifndef SETTINGS_H
#define SETTINGS_H

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

class Settings
{
public:
	Settings();
	JsonArray read(const char *key, const JsonArray defaultValue);
	const char *read(const char *key, const char *defaultValue);
	double read(const char *key, const double defaultValue);
	void write(JsonDocument input);
	static void clear();
	static void sendToServer(ESP8266WebServer &server);

private:
	JsonDocument jsonDocument;
	DeserializationError deserializationError;
};

#endif
