#ifndef ROUNDROBINSCHEDULER_H
#define ROUNDROBINSCHEDULER_H

#include "Component.hpp"
#include "Connection.hpp"
#include "Telemetry.hpp"
#include "Display.hpp"
#include "TelemetryData.hpp"

#include <array>

namespace ah
{
    class RoundRobinScheduler
    {
    public:
        void Init();
        void Update();

    private:
        void GatherComponents();
        void InitTimer();

        void UpdateEveryTickComponents();
        void UpdateComponentsByIndex();

        static void IRAM_ATTR OnTimer();

    private:
        Connection _connection;
        Telemetry _telemetry;
        Display _display;

        std::array<Component*, 3> _components;
        
        hw_timer_t* _timer = nullptr;
        size_t _index = 0;

        TelemetryData _telemetryData;

        static volatile bool _needUpdate;
    };
}

#endif