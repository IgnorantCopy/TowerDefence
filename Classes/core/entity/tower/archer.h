#ifndef TOWERDEFENCE_ARCHER_H
#define TOWERDEFENCE_ARCHER_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct Archer final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        Archer(id::Id id, const timer::Clock & clk);
        Archer() = delete;

        static constexpr TowerInfo INFO { 2400, 18, 0, 25, 3, AttackType::Physics, TowerType::Archer};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_ARCHER_H
