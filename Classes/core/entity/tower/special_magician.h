#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
namespace core {

struct SpecialMagician final : Tower {

    bool is_attacking = false;
    size_t silence_count = 0;
    timer::Timer update_silence_count;
    timer::Timer release_skill;
    bool has_skill = false;

    SpecialMagician(id::Id id, const timer::Clock &clk)
        : Tower(id, clk), release_skill(clk.with_period_sec(10)),
          update_silence_count{clk.with_period_sec(3)} {}

    static constexpr TowerInfo INFO{1400, 14, 0, 30, 2, AttackType::Magic};
    TowerInfo info() const override { return INFO; }

    TowerInfo status() const override {
        auto status = Tower::status();

        status.attack_ *= 1 + this->silence_count * 0.5;

        return status;
    }

    void on_tick(GridRef g) override;
};
} // namespace core
} // namespace towerdefence

#endif // TOWERDEFENCE_SPECIAL_MAGICIAN_H
