#ifndef CONNECTION_H
#define CONNECTION_H

#include "Component.hpp"

namespace ah
{
    struct TelemetryData;

    class Connection final
        : public Component
    {
    public:
        void Init() override;
        void HandleTelemetry(TelemetryData& data) override;
    };
}

#endif