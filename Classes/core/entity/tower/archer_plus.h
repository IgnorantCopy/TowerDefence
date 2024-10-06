#ifndef TOWERDEFENCE_ARCHER_PLUS_H
#define TOWERDEFENCE_ARCHER_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct archerPlus final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        archerPlus(id::Id id, const timer::Clock & clk);
        archerPlus() = delete;

        TowerInfo info() const override { return { 2800, 24, 0, 10, AttackType::Physics };}

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_ARCHER_PLUS_H
