#ifndef TOWERDEFENCE_BOSS_2_H
#define TOWERDEFENCE_BOSS_2_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct Boss2 final : Enemy {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        Boss2(id::Id id, const timer::Clock & clk);
        Boss2() = delete;

        EnemyInfo info() const override {
            return { 70000, { 1500, 60 }, 5 };
        }

        void on_tick(GridRef g) override;
        void on_death(GridRef g) override;
    };
} // namespace towerdefence::core

#endif //TOWERDEFENCE_BOSS_2_H
