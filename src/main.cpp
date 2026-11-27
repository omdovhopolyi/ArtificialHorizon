#include <Arduino.h>

#include "Connection.hpp"
#include "Display.hpp"
#include "Telemetry.hpp"

#define INTERVAL_MS_PRINT 100

ah::Connection connection;
ah::Display display;
ah::Telemetry telemetry;

unsigned long lastPrintMillis = 0; 

void setup() {
  	Serial.begin(115200);

	connection.Init();
	telemetry.Init();
	display.Init();
}

void loop() {
	unsigned long now = millis(); 
	telemetry.Update();
	
	if (now - lastPrintMillis > INTERVAL_MS_PRINT) {
		ah::TelemetryData packet;
		telemetry.HandleTelemetry(packet);
		display.HandleTelemetry(packet);
		connection.HandleTelemetry(packet);

		lastPrintMillis = now; 
	}
}
