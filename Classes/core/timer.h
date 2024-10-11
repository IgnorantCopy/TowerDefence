#ifndef TOWERDEFENCE_CORE_TIMER
#define TOWERDEFENCE_CORE_TIMER

#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <unordered_map>
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

    struct hasher {
        size_t operator()(const Timer &t) const noexcept {
            return std::visit(
                overloaded{[](const Period &p) {
                               return std::hash<uint64_t>{}(
                                   uint64_t(p.period) << 32 | p.start);
                           },
                           [](const Duration &p) {
                               return std::hash<uint64_t>{}(
                                   uint64_t(p.duration) << 32 | p.start);
                           }},
                t.clk);
        }
    };

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

struct CallbackTimer {
    std::unordered_map<Timer, std::deque<std::function<bool()>>, Timer::hasher>
        cbs;

    // add a callback called when t tiggers.
    // if callback returns false, it will be removed.
    //
    // SAFETY: caller must ensure that all captured variables of callback's
    // lifetime NOT SHORTER than the object.
    void add_callback(Timer t, std::function<bool()> callback) {
        cbs[t].push_back(callback);
    }

    void on_tick(const Clock &clk) {
        for (auto &[timer, cbs] : this->cbs) {
            if (clk.is_triggered(timer)) {
                for (auto it = cbs.begin(); it != cbs.end(); ++it) {
                    auto ret = (*it)();
                    if (!ret) {
                        cbs.erase(it);
                    }
                }

                // todo: remove timer that will never get tiggered again.
            }
        }
    }
};

} // namespace towerdefence::core::timer

#endif