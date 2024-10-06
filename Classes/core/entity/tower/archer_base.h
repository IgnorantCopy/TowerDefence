#ifndef TOWERDEFENCE_ARCHER_BASE_H
#define TOWERDEFENCE_ARCHER_BASE_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct archorBase final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        archorBase(id::Id id, const timer::Clock & clk);
        archorBase() = delete;

        TowerInfo info() const override { return { 500, 7, 10, 15, AttackType::Physics }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core

#endif //TOWERDEFENCE_ARCHER_BASE_H
