#ifndef CONNECTION_H
#define CONNECTION_H

namespace ah
{
    struct TelemetryData;

    class Connection
    {
    public:
        void Init();
        void HandleTelemetry(TelemetryData& data);
    };
}

#endif