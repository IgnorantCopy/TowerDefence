#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct HighspeedArcher final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        HighspeedArcher(id::Id id, const timer::Clock & clk);
        HighspeedArcher() = delete;

        TowerInfo info() const override { return { 1000, 10, 0, 10, 2, AttackType::Physics }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_H
