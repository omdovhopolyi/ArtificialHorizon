#include "RoundRobinScheduler.hpp"
#include "Config.hpp"
#include "ErrorHandler.hpp"
#include <esp_arduino_version.h>

namespace ah
{
    volatile bool RoundRobinScheduler::_needUpdate = true;

    void RoundRobinScheduler::Init()
    {
        GatherComponents();
        InitTimer();
    }

    void RoundRobinScheduler::Update()
    {
        UpdateEveryTickComponents();
        UpdateComponentsByIndex();
    }

    void RoundRobinScheduler::OnReset()
    {
        _connection.HandleReset();
    }

    void RoundRobinScheduler::GatherComponents()
    {
        _components[0] = &_telemetry;
        _components[1] = &_display;
        _components[2] = &_connection;

        for (const auto component : _components) {
            component->Init();
        }
    }

    void RoundRobinScheduler::UpdateEveryTickComponents()
    {
        _telemetry.Update();
    }

    void RoundRobinScheduler::UpdateComponentsByIndex()
    {
        if (_needUpdate) {
            _needUpdate = false;

            if (_index >= _components.size()) {
                ErrorHandler("[RoundRobinScheduler::Update] invalid index");
            }

            Component* component = _components[_index];
            component->HandleTelemetry(_telemetryData);

            _index++;
            _index %= _components.size();
        }
    }

    void IRAM_ATTR RoundRobinScheduler::OnTimer()
    {
        _needUpdate = true;
    }

    void RoundRobinScheduler::InitTimer()
    {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
        _timer = timerBegin(Config::TimerFrequency);
        timerAttachInterrupt(_timer, &RoundRobinScheduler::OnTimer);
        timerAlarm(_timer, Config::TimerAlarmPeriod, true, 0);
#else
        _timer = timerBegin(0, 80, true);
        timerAttachInterrupt(_timer, &RoundRobinScheduler::OnTimer, true);
        timerAlarmWrite(_timer, Config::TimerAlarmPeriod, true);
        timerAlarmEnable(_timer);
#endif
    }
}
