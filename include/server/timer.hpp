#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <memory>

using TimerID = std::uint32_t;
using TimerCallback = std::function<void()>;

class Timer
{
public:
    Timer();
    ~Timer();

    TimerID set_timeout(float seconds, TimerCallback callback);
    TimerID set_interval(float seconds, TimerCallback callback);
    void clear_timer(TimerID timer_id);
    void update();
    void pause();
    void resume();
    float get_elapsed_time() const;

private:
    struct TimerData
    {
        std::chrono::steady_clock::time_point next_trigger;
        float interval;
        TimerCallback callback;
        bool is_repeating;
    };

    std::map<TimerID, TimerData> timers_;
    TimerID next_timer_id_{1};
    bool is_paused_{false};
    std::chrono::steady_clock::time_point last_update_;
    float elapsed_time_{0.0f};

    TimerID generate_timer_id();
};