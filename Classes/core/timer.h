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

constexpr uint32_t TICK_PER_SECOND = 30;

template <class... Ts> struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <class T> static inline auto cmp(const T &val) {
    return [&val](const T &rhs) { return rhs == val; };
}

static inline auto def(auto val) {
    return [val](auto &&) { return val; };
}

template <class T>
static inline void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct Timer {
    struct Period {
        uint32_t period = 1;
        uint32_t start = 0;

        bool operator==(const Period &rhs) const = default;

        bool operator==(const Timer &rhs) const noexcept {
            return std::visit(overloaded{cmp<Period>(*this), def(false)},
                              rhs.clk);
        }
    };

    struct Duration {
        uint32_t duration = 0;
        uint32_t start = 0;

        bool operator==(const Duration &rhs) const = default;

        bool operator==(const Timer &rhs) const noexcept {
            return std::visit(overloaded{cmp<Duration>(*this), def(false)},
                              rhs.clk);
        }
    };

    struct Before {
        uint32_t start;
        uint32_t duration;
        uint32_t period;

        bool operator==(const Before &rhs) const = default;

        bool operator==(const Timer &rhs) const noexcept {
            return std::visit(overloaded{cmp<Before>(*this), def(false)},
                              rhs.clk);
        }
    };

    struct Never : std::monostate {
        bool operator==(const Never &rhs) const = default;

        bool operator==(const Timer &rhs) const noexcept {
            return std::visit(overloaded{cmp<Never>(*this), def(false)},
                              rhs.clk);
        }
    };

    std::variant<Period, Duration, Before, Never> clk;

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
                           [](const Before &b) {
                               size_t seed = 0;
                               hash_combine(seed, b.start);
                               hash_combine(seed, b.duration);
                               hash_combine(seed, b.period);
                               return seed;
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

    constexpr static Timer before(uint32_t duration, uint32_t period,
                                  uint32_t start) {
        return {Before{start, duration, period}};
    }

    constexpr static Timer never() { return {.clk = Never{}}; }

    template <class T, class F>
        requires std::is_invocable_r_v<T, F, Period &>
    T visit_period(F f, T val = {}) {
        return std::visit(overloaded{f, def(val)}, clk);
    }

    template <class F>
        requires std::is_invocable_v<F, Period &>
    void visit_period(F f) {
        return std::visit(overloaded{f, [](auto &&) {}}, clk);
    }

    bool operator==(const Timer &rhs) const noexcept {
        return std::visit(overloaded{[&](auto &&v) { return v == rhs; }},
                          rhs.clk);
    }
};

struct Clock {
    uint32_t elapased_ = 0;

    void on_tick() { elapased_ += 1; }

    bool is_triggered(Timer clk) const {
        return std::visit(
            overloaded{[this](const Timer::Period &p) {
                           return (elapased_ - p.start) % ((p.period) ? p.period : 1) == 0;
                       },
                       [this](const Timer::Duration &d) {
                           return d.duration == elapased_ - d.start;
                       },
                       [this](const Timer::Before &b) {
                           return (elapased_ - b.start) <= b.duration &&
                                  (elapased_ - b.start) % b.period == 0;
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

    Timer with_before(uint32_t duration, uint32_t period = 1) const {
        return Timer::before(duration, period, this->elapased_);
    }

    Timer with_before_sec(uint32_t secs, uint32_t period = 1) const {
        return this->with_before(secs * 30, period * 30);
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
                for (auto it = cbs.begin(); it != cbs.end();) {
                    auto ret = (*it)(std::forward<Args>(parmas)...);
                    if (!ret) {
                        it = cbs.erase(it);
                    } else {
                        ++it;
                    }
                }

                // todo: remove timer that will never get tiggered again.
            }
        }
    }
};

} // namespace towerdefence::core::timer

#endif
