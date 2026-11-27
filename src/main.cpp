#include <Arduino.h>
// #include <MPU9250.h>
#include <Wire.h>

#include "Config.hpp"
#include "Display.hpp"
#include "Telemetry.hpp"

#define INTERVAL_MS_PRINT 100
Display display;
Telemetry telemetry;

unsigned long lastPrintMillis = 0; 

void setup() {
  	Serial.begin(115200);
  	Serial1.begin(115200, SERIAL_8N1, Config::PinRX, Config::PinTX);

	telemetry.Init();
	display.Init();
}

void loop() {
	unsigned long now = millis(); 
	telemetry.Update();
	
	if (now - lastPrintMillis > INTERVAL_MS_PRINT) { 
		TelemetryData packet;
		telemetry.HandleTelemetry(packet);
		display.HandleTelemetry(packet);
		Serial1.write(reinterpret_cast<const uint8_t*>(&packet), sizeof(packet));

		lastPrintMillis = now; 
	}
}
