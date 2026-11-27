#ifndef TELEMETRYDATA_H
#define TELEMETRYDATA_H

#include <stdint.h>

struct TelemetryData {
	float pitch = 0.f;
	float roll = 0.f;
	uint32_t timestamp = 0UL;
};

#endif