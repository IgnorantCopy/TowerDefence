#ifndef TOWERDEFENCE_ARCHER_H
#define TOWERDEFENCE_ARCHER_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct archer final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        archer(id::Id id, const timer::Clock & clk);
        archer() = delete;

        TowerInfo info() const override { return { 2400, 18, 0, 10, AttackType::Physics }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_ARCHER_H
