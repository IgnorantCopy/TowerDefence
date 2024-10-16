#ifndef TOWERDEFENCE_ARCHER_PLUS_H
#define TOWERDEFENCE_ARCHER_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct ArcherPlus final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        ArcherPlus(id::Id id, const timer::Clock & clk);
        ArcherPlus() = delete;

        static constexpr TowerInfo INFO { 2800, 24, 0, 25, 3, AttackType::Physics, TowerType::ArcherPlus};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_ARCHER_PLUS_H
