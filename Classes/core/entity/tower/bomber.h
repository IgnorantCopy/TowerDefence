#ifndef TOWERDEFENCE_BOMBER_H
#define TOWERDEFENCE_BOMBER_H

#include "../../id.h"
#include "../entity.h"

namespace towerdefence::core {
    struct Bomber final : Tower {
        timer::Timer release_skill_;

        Bomber(id::Id id, const timer::Clock & clk);
        Bomber() = delete;

        static constexpr TowerInfo INFO { 1600, 16, 0, 20, 4, AttackType::Physics, TowerType::Bomber};
        TowerInfo info() const override { return INFO; }

        bool skill = false;

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_BOMBER_H
