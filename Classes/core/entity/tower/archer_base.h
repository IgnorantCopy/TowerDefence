#ifndef TOWERDEFENCE_ARCHER_BASE_H
#define TOWERDEFENCE_ARCHER_BASE_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct ArcherBase final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        ArcherBase(id::Id id, const timer::Clock & clk);
        ArcherBase() = delete;

        static constexpr TowerInfo INFO { 500, 7, 10, 15, 2, AttackType::Physics, TowerType::ArcherBase};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core

#endif //TOWERDEFENCE_ARCHER_BASE_H
