#ifndef CONNECTION_H
#define CONNECTION_H

struct TelemetryData;

class Connection
{
public:
    void Init();
    void HandleTelemetry(TelemetryData& data);

private:

};

#endif