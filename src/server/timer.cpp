#include "server/timer.hpp"

Timer::Timer() : last_update_(std::chrono::steady_clock::now()) {}

Timer::~Timer() = default;

TimerID Timer::set_timeout(float seconds, TimerCallback callback)
{
    TimerID id = generate_timer_id();
    auto now = std::chrono::steady_clock::now();
    TimerData timer_data{
        now + std::chrono::microseconds(static_cast<int64_t>(seconds * 1000000)),
        seconds,
        std::move(callback),
        false};
    timers_[id] = std::move(timer_data);
    return id;
}

TimerID Timer::set_interval(float seconds, TimerCallback callback)
{
    TimerID id = generate_timer_id();
    auto now = std::chrono::steady_clock::now();
    TimerData timer_data{
        now + std::chrono::microseconds(static_cast<int64_t>(seconds * 1000000)),
        seconds,
        std::move(callback),
        true};
    timers_[id] = std::move(timer_data);
    return id;
}

void Timer::clear_timer(TimerID timer_id)
{
    timers_.erase(timer_id);
}

void Timer::update()
{
    if (is_paused_)
    {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - last_update_;
    elapsed_time_ += elapsed.count();
    last_update_ = now;

    std::vector<TimerID> completed_timers;

    for (auto &[id, timer] : timers_)
    {
        if (now >= timer.next_trigger)
        {
            timer.callback();

            if (timer.is_repeating)
            {
                // Schedule next trigger
                timer.next_trigger += std::chrono::microseconds(
                    static_cast<int64_t>(timer.interval * 1000000));
            }
            else
            {
                completed_timers.push_back(id);
            }
        }
    }

    // Remove completed non-repeating timers
    for (auto id : completed_timers)
    {
        timers_.erase(id);
    }
}

void Timer::pause()
{
    if (!is_paused_)
    {
        is_paused_ = true;
    }
}

void Timer::resume()
{
    if (is_paused_)
    {
        is_paused_ = false;
        last_update_ = std::chrono::steady_clock::now();
    }
}

float Timer::get_elapsed_time() const
{
    return elapsed_time_;
}

TimerID Timer::generate_timer_id()
{
    return next_timer_id_++;
}