#ifndef CONNECTION_H
#define CONNECTION_H

#include "Component.hpp"
#include <stdint.h>

namespace ah
{
    struct TelemetryData;

    enum class Command : uint8_t
    {
        Point = 0x01,
        Reset = 0x02
    };

    class Connection final
        : public Component
    {
    public:
        void Init() override;

        void HandleReset();
        void HandleTelemetry(TelemetryData& data) override;

    private:
        void SerializeResetCommand();
        void SerializeTelemetryData(const TelemetryData& data);

        void SerializeInt8(uint8_t value, uint8_t& index);
        void SerializeInt16(uint16_t value, uint8_t& index);
        void SerializeInt32(uint32_t value, uint8_t& index);
        void SerializeFloat(float value, uint8_t& index);

    private:
        uint8_t _buffer[15];
    };
}

#endif