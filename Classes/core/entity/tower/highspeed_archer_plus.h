#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H

#include "../../id.h"
#include "../entity.h"

namespace towerdefence::core {
    struct HighspeedArcherPlus final : Tower {
        timer::Timer release_skill_;

        HighspeedArcherPlus(id::Id id, const timer::Clock & clk);
        HighspeedArcherPlus() = delete;

        static constexpr TowerInfo INFO { 1200, 15, 0, 10, 2, AttackType::Physics, TowerType::HighspeedArcherPlus};
        TowerInfo info() const override { return INFO; }

        bool skill = false;

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
