#ifndef TOWERDEFENCE_CORE_TIMER
#define TOWERDEFENCE_CORE_TIMER

#include <cstdint>
#include <variant>

namespace towerdefence::core::timer {

template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct Clock {
    struct Period {
        uint32_t period = 1;
        uint32_t start = 0;
    };

    struct Duration {
        uint32_t duration = 0;
        uint32_t start = 0;
    };

    std::variant<Period, Duration> clk;

    constexpr static Clock period(uint32_t period, uint32_t start) {
        return {.clk = Period{.period = period, .start = start}};
    }

    constexpr static Clock duration(uint32_t duration, uint32_t start) {
        return {.clk = Duration{.duration = duration, .start = start}};
    }
};

struct Timer {
    uint32_t elapased_ = 0;

    void on_tick() { elapased_ += 1; }

    bool is_triggered(Clock clk) const {
        return std::visit(
            overloaded{[this](const Clock::Period &p) {
                           return (elapased_ - p.start) % p.period == 0;
                       },
                       [this](const Clock::Duration &d) {
                           return d.duration == elapased_ - d.start;
                       }},
            clk.clk);
    }

    // return a clock that can be notified after `duration`
    Clock after(uint32_t duration) const {
        return Clock::duration(duration, elapased_);
    }

    // return a clock that can be continously notified after `p`
    Clock with_period(uint32_t p) const { return Clock::period(p, elapased_); }
};

} // namespace towerdefence::core::timer

#endif