#include <Arduino.h>
#include "RoundRobinScheduler.hpp"

ah::RoundRobinScheduler scheduler;

void setup() {
  	Serial.begin(115200);
	scheduler.Init();
}

void loop() {
	scheduler.Update();
}
