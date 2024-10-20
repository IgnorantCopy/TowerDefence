#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"

namespace towerdefence::core {
    struct DiffusiveMagician final : Tower {
        timer::Timer release_skill_;

        DiffusiveMagician(id::Id id, const timer::Clock & clk);
        DiffusiveMagician() = delete;

        static constexpr TowerInfo INFO { 1500, 15, 0, 25, 4, AttackType::Magic, TowerType::DiffusiveMagician};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
