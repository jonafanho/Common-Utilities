#ifndef CUSTOM_SCHEDULER_H
#define CUSTOM_SCHEDULER_H

#include <stdint.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define MAX_COUNT 64
#define MAX_CHANNELS 8

class CustomScheduler
{
public:
	void setup();
	uint8_t tick();
	void add(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel);
	void remove(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel);

private:
	int times[MAX_COUNT];
	uint8_t channels[MAX_COUNT];
	uint8_t count;
	int previousTime;

	void writeSettings();
	static int getTime(uint8_t hour, uint8_t minute, uint8_t second);
};

#endif
