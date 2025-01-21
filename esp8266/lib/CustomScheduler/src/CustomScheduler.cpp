#include "CustomScheduler.h"
#include <HttpRequest.h>
#include <Settings.h>
#include <NTPClient.h>

WiFiUDP wiFiUDP;
NTPClient ntpClient(wiFiUDP, "pool.ntp.org");

void CustomScheduler::setup()
{
	Settings settings;
	JsonArray scheduleArray = settings.read("custom-scheduler", JsonArray());
	for (uint8_t i = 0; i < std::min((uint8_t)scheduleArray.size(), (uint8_t)MAX_COUNT); i++)
	{
		times[i] = scheduleArray[i]["time"];
		channels[i] = scheduleArray[i]["channel"];
		count++;
	}

	ntpClient.begin();
	ntpClient.update();
	previousTime = getTime(ntpClient.getHours(), ntpClient.getMinutes(), ntpClient.getSeconds());
}

uint8_t CustomScheduler::tick()
{
	ntpClient.update();
	int time = getTime(ntpClient.getHours(), ntpClient.getMinutes(), ntpClient.getSeconds());
	uint8_t total = 0;

	if (previousTime != time)
	{
		for (uint8_t i = 0; i < count; i++)
		{
			if ((previousTime < times[i] || previousTime > time) && time >= times[i])
			{
				total = total | (1 << (channels[i] - 1));
			}
		}
	}

	previousTime = time;
	return total;
}

void CustomScheduler::add(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel)
{
	if (channel > 0 && channel <= MAX_CHANNELS)
	{
		int time = getTime(hour, minute, second);
		for (uint8_t i = 0; i < count; i++)
		{
			if (time == times[i] && channel == channels[i])
			{
				return;
			}
		}
		if (count < MAX_COUNT)
		{
			times[count] = time;
			channels[count] = channel;
			count++;
			writeSettings();
		}
	}
}

void CustomScheduler::remove(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel)
{
	int time = getTime(hour, minute, second);
	for (uint8_t i = 0; i < count; i++)
	{
		if (time == times[i] && channel == channels[i])
		{
			count--;
			times[i] = times[count];
			channels[i] = channels[count];
			writeSettings();
			break;
		}
	}
}

void CustomScheduler::writeSettings()
{
	JsonDocument scheduleArray;

	for (uint8_t i = 0; i < count; i++)
	{
		JsonDocument scheduleObject;
		scheduleObject["time"] = times[i];
		scheduleObject["channel"] = channels[i];
		scheduleArray.add(scheduleObject);
	}

	JsonDocument mainObject;
	mainObject["custom-scheduler"] = scheduleArray;
	Settings settings;
	settings.write(mainObject);
}

int CustomScheduler::getTime(uint8_t hour, uint8_t minute, uint8_t second)
{
	return (hour * 3600 + minute * 60 + second) * 1000;
}
