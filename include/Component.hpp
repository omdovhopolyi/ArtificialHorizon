#ifndef COMPONENT_H
#define COMPONENT_H

namespace ah
{
    struct TelemetryData;

    class Component
    {
    public:
        virtual ~Component() = default; 

        virtual void Init() = 0;
        virtual void HandleTelemetry(TelemetryData& data) = 0;
    };
}

#endif