#ifndef TOWERDEFENCE_CORE_TIMER
#define TOWERDEFENCE_CORE_TIMER

#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <utility>
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

    struct Never : std::monostate {};

    std::variant<Period, Duration, Never> clk;

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
                           },
                           [](const Never &n) {
                               return std::hash<std::monostate>{}(n);
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

    constexpr static Timer never() { return {.clk = Never{}}; }

    template <class T, class F>
        requires std::is_invocable_r_v<T, F, Period &>
    T visit_period(F f, T val = {}) {
        return std::visit(overloaded{f, [val](auto &&) { return val; }}, clk);
    }

    template <class F>
        requires std::is_invocable_v<F, Period &>
    void visit_period(F f) {
        return std::visit(overloaded{f, [](auto &&) {}}, clk);
    }

    bool operator==(const Timer &rhs) const noexcept {
        return std::visit(
            overloaded{
                [&](const Period &p) {
                    return std::visit(overloaded{[&](const Period &p2) {
                                                     return p.period ==
                                                                p2.period &&
                                                            p.start == p2.start;
                                                 },
                                                 [](auto &&) { return false; }},
                                      rhs.clk);
                },
                [&](const Duration &d) {
                    return std::visit(overloaded{[&](const Duration &d2) {
                                                     return d.duration ==
                                                                d2.duration &&
                                                            d.start == d2.start;
                                                 },
                                                 [](auto &&) { return false; }},
                                      rhs.clk);
                },
                [&](const Never &) {
                    return std::visit(
                        overloaded{[&](const Never &d2) { return true; },
                                   [](auto &&) { return false; }},
                        rhs.clk);
                }},
            rhs.clk);
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
                       },
                       [](const Timer::Never &) { return false; }},
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

    Timer never() const { return Timer::never(); }
};

template <class... Args> struct CallbackTimer {
    using Callback = std::function<bool(Args...)>;

    std::unordered_map<Timer, std::deque<Callback>, Timer::hasher> cbs;

    // add a callback called when t tiggers.
    // if callback returns false, it will be removed.
    //
    // SAFETY: caller must ensure that all captured variables of callback's
    // lifetime NOT SHORTER than the object.
    void add_callback(Timer t, Callback callback) {
        cbs[t].push_back(callback);
    }

    void on_tick(const Clock &clk, Args... parmas) {
        for (auto &[timer, cbs] : this->cbs) {
            if (clk.is_triggered(timer)) {
                for (auto it = cbs.begin(); it != cbs.end(); ++it) {
                    auto ret = (*it)(std::forward<Args>(parmas)...);
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