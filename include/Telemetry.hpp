#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include <MPU9250.h>
#include "Component.hpp"

namespace ah
{
    class Telemetry final
        : public Component
    {
    public:
        void Init() override;
        void Update();
        void HandleTelemetry(TelemetryData& data) override;

    private:
        MPU9250 _mpu;
    };
}

#endif