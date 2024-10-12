#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
namespace core {

struct DiffusiveMagicianPlus final : Tower,
                                     TimeOutMixin<DiffusiveMagicianPlus> {

    timer::Timer release_skill_;

    DiffusiveMagicianPlus(id::Id id, const timer::Clock &clk)
        : Tower{id, clk}, release_skill_(clk.with_period_sec(40)) {}

    static constexpr TowerInfo INFO{2000, 21, 0, 20, 4, AttackType::Magic};
    TowerInfo info() const override { return INFO; }

    void on_tick(GridRef g) override;
};
} // namespace core
} // namespace towerdefence

#endif // TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
