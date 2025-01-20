#ifndef CUSTOM_SCHEDULER_H
#define CUSTOM_SCHEDULER_H

#include <stdint.h>

#define MAX_COUNT 32

class CustomScheduler
{
public:
	void setup();
	int16_t tick();
	void add(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel);
	void remove(uint8_t hour, uint8_t minute, uint8_t second, uint8_t channel);

private:
	int times[MAX_COUNT];
	uint8_t channels[MAX_COUNT];
	uint8_t count;

	void writeSettings();
};

#endif
