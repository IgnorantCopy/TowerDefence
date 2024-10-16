#ifndef TOWERDEFENCE_BOSS_1_H
#define TOWERDEFENCE_BOSS_1_H

#include "../../id.h"
#include "../entity.h"
#include "../route.h"

namespace towerdefence::core {
    struct Boss1 final : Enemy {
        timer::Timer release_skill_;

        Boss1(id::Id id, route::Route route, const timer::Clock & clk);

        EnemyInfo info() const override {
            return { 50000, { 800, 30 }, 5, EnemyType::Boss1, 90};
        }

        void on_tick(GridRef g) override;
        void on_death(GridRef g) override;
    };
} // namespace towerdefence::core

#endif //TOWERDEFENCE_BOSS_1_H
