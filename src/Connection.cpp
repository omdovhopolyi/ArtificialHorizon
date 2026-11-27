#include "Connection.hpp"
#include "Config.hpp"
#include "TelemetryData.hpp"
#include <Arduino.h>

namespace ah
{
    void Connection::Init()
    {
        Serial1.begin(Config::UARTBaud, SERIAL_8N1, Config::PinRX, Config::PinTX);
    }

    void Connection::HandleTelemetry(TelemetryData& data)
    {
        Serial1.write(reinterpret_cast<const uint8_t*>(&data), sizeof(data));
    }
}
