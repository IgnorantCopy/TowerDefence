#ifndef TOWERDEFENCE_CORE_TIMER
#define TOWERDEFENCE_CORE_TIMER

#include <cstdint>
#include <variant>

namespace towerdefence::core::timer {

template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct Timer {
    struct Period {
        uint32_t period = 1;
        uint32_t start = 0;
    };

    struct Duration {
        uint32_t duration = 0;
        uint32_t start = 0;
    };

    std::variant<Period, Duration> clk;

    constexpr static Timer period(uint32_t period, uint32_t start) {
        return {Period{period, start}};
    }

    constexpr static Timer duration(uint32_t duration, uint32_t start) {
        return {Duration{duration, start}};
    }
};

struct Clock {
    uint32_t elapased_ = 0;

    void on_tick() { elapased_ += 1; }

    bool is_triggered(Timer clk) const {
        return std::visit(
            overloaded{[this](const Timer::Period &p) {
                           return (elapased_ - p.start) % p.period == 0;
                       },
                       [this](const Timer::Duration &d) {
                           return d.duration == elapased_ - d.start;
                       }},
            clk.clk);
    }

    // return a clock that can be notified after `duration`
    Timer with_duration(uint32_t duration) const {
        return Timer::duration(duration, elapased_);
    }

    Timer with_duration_sec(uint32_t secs) const {
        return this->with_duration(secs * 30);
    }

    // return a clock that can be continously notified after `p`
    Timer with_period(uint32_t p) const { return Timer::period(p, elapased_); }

    Timer with_period_sec(uint32_t secs) const {
        return this->with_period(secs * 30);
    }
};

} // namespace towerdefence::core::timer

#endif