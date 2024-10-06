#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct highspeedArcher final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        highspeedArcher(id::Id id, const timer::Clock & clk);
        highspeedArcher() = delete;

        TowerInfo info() const override { return { 1000, 10, 0, 10, AttackType::Physics }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_H
