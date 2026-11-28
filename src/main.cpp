#include <Arduino.h>
#include "RoundRobinScheduler.hpp"

ah::RoundRobinScheduler scheduler;

void setup() {
  	Serial.begin(115200);
	scheduler.Init();
	scheduler.OnReset();
}

void loop() {
	scheduler.Update();
}
