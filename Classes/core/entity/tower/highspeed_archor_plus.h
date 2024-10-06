#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct highspeedArcherPlus final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        highspeedArcherPlus(id::Id id, const timer::Clock & clk);
        highspeedArcherPlus() = delete;

        TowerInfo info() const override { return { 1300, 15, 0, 10, AttackType::Physics }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
