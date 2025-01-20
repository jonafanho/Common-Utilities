#include "Settings.h"
#include <LittleFS.h>

#define SETTINGS_FILE "/settings.json"

Settings::Settings()
{
	File file = LittleFS.open(SETTINGS_FILE, "r");
	if (file)
	{
		deserializationError = deserializeJson(jsonDocument, file.readString());
		file.close();
	}
}

JsonArray Settings::read(const char *key, const JsonArray defaultValue)
{
	if (deserializationError)
	{
		return defaultValue;
	}
	else
	{
		return jsonDocument[key] | defaultValue;
	}
}

double Settings::read(const char *key, const double defaultValue)
{
	if (deserializationError)
	{
		return defaultValue;
	}
	else
	{
		return jsonDocument[key] | defaultValue;
	}
}

const char *Settings::read(const char *key, const char *defaultValue)
{
	if (deserializationError)
	{
		return defaultValue;
	}
	else
	{
		return jsonDocument[key] | defaultValue;
	}
}

void Settings::write(JsonDocument input)
{
	for (JsonPairConst jsonPairConst : input.as<JsonObjectConst>())
	{
		jsonDocument[jsonPairConst.key()] = jsonPairConst.value();
	}

	File file = LittleFS.open(SETTINGS_FILE, "w");
	if (file)
	{
		serializeJson(jsonDocument, file);
		file.close();
	}
}

void Settings::clear()
{
	File file = LittleFS.open(SETTINGS_FILE, "w");
	if (file)
	{
		file.print("{}");
		file.close();
	}
}

void Settings::sendToServer(ESP8266WebServer &server)
{
	File file = LittleFS.open(SETTINGS_FILE, "r");
	if (file)
	{
		server.send(200, "application/json", file.readString());
		file.close();
	}
	else
	{
		server.send(200, "application/json", "{}");
	}
}
