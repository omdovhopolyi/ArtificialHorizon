#include "Connection.hpp"
#include "Config.hpp"
#include "TelemetryData.hpp"
#include <Arduino.h>

namespace ah
{
    static constexpr uint16_t Header = 0xAA55;

    void Connection::Init()
    {
        Serial1.begin(Config::UARTBaud, SERIAL_8N1, Config::PinRX, Config::PinTX);
    }

    void Connection::HandleReset()
    {
        SerializeResetCommand();
        Serial1.write(_buffer, sizeof(_buffer));
    }

    void Connection::HandleTelemetry(TelemetryData& data)
    {
        SerializeTelemetryData(data);
        Serial1.write(_buffer, sizeof(_buffer)); // TODO buffer size
    }

    void Connection::SerializeResetCommand()
    {
        uint8_t byteIndex = 0;
        SerializeInt16(Header, byteIndex);
        SerializeInt8(static_cast<uint8_t>(Command::Reset), byteIndex);
    }

    void Connection::SerializeTelemetryData(const TelemetryData& data)
    {
        uint8_t byteIndex = 0;
        SerializeInt16(Header, byteIndex);
        SerializeInt8(static_cast<uint8_t>(Command::Point), byteIndex);
        SerializeFloat(data.pitch, byteIndex);
        SerializeFloat(data.roll, byteIndex);
        SerializeInt32(data.timestamp, byteIndex);
    }

    void Connection::SerializeInt8(uint8_t value, uint8_t& index)
    {
        _buffer[index++] = value;
    }

    void Connection::SerializeInt16(uint16_t value, uint8_t& index)
    {
        _buffer[index++] = value >> 8;
        _buffer[index++] = value;
    }

    void Connection::SerializeInt32(uint32_t value, uint8_t& index)
    {
        _buffer[index++] = value >> 24;
        _buffer[index++] = value >> 16;
        _buffer[index++] = value >> 8;
        _buffer[index++] = value;
    }

    void Connection::SerializeFloat(float value, uint8_t& index)
    {
        uint32_t intValue;
        memcpy(&intValue, &value, sizeof(uint32_t));
        SerializeInt32(intValue, index);
    }
}
