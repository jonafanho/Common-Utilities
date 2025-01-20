#ifndef SETTINGS_H
#define SETTINGS_H

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <ESP8266WebServer.h>

#define SETTINGS_FILE "/settings.json"

class Settings
{
public:
	Settings()
	{
		File file = LittleFS.open(SETTINGS_FILE, "r");
		if (file)
		{
			deserializationError = deserializeJson(jsonDocument, file.readString());
			file.close();
		}
	}

	static void setup()
	{
		LittleFS.begin();
	}

	template <typename T>
	T read(const char *key, const T defaultValue)
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

	void write(JsonObjectConst input)
	{
		for (JsonPairConst jsonPairConst : input)
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

	static void clear()
	{
		File file = LittleFS.open(SETTINGS_FILE, "w");
		if (file)
		{
			file.print("{}");
			file.close();
		}
	}

	static void sendToServer(ESP8266WebServer &server)
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

private:
	JsonDocument jsonDocument;
	DeserializationError deserializationError;
};

#endif
