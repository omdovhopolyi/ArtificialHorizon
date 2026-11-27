#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include <MPU9250.h>
#include "TelemetryData.hpp"

namespace ah
{
    class Telemetry
    {
    public:
        void Init();
        void Update();
        void HandleTelemetry(TelemetryData& data);

    private:
        MPU9250 _mpu;
    };
}

#endif