#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
namespace core {

struct DiffusiveMagicianPlus final : Tower {
    timer::Timer attack_;
    timer::CallbackTimer<DiffusiveMagicianPlus&, GridRef> timeouts_;

    DiffusiveMagicianPlus(id::Id id, const timer::Clock &clk)
        : Tower{id}, attack_{clk.with_period_sec(INFO.attack_interval_)} {}

    static constexpr TowerInfo INFO{2000, 21, 0, 20, 4, AttackType::Magic};
    TowerInfo info() const override { return INFO; }

    void on_tick(GridRef g) override;
};
} // namespace core
} // namespace towerdefence

#endif // TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
