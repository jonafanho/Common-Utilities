#include "CustomScheduler.h"
#include <Settings.h>

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
}

int16_t CustomScheduler::tick()
{
	return -1;
}

void CustomScheduler::add(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel)
{
	int time = (hour * 3600 + minute * 60 + second) * 1000;
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

void CustomScheduler::remove(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel)
{
	int time = (hour * 3600 + minute * 60 + second) * 1000;
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
