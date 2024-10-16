#ifndef TOWERDEFENCE_BOSS_2_H
#define TOWERDEFENCE_BOSS_2_H

#include "../../id.h"
#include "../entity.h"
#include "../route.h"

namespace towerdefence::core {
    struct Boss2 final : Enemy {
        struct {
            timer::Timer dec_atk_spd;
            timer::Timer silent;
            timer::Timer withdraw;
        } release_skill_;

        Boss2(id::Id id, route::Route route, const timer::Clock & clk);
        Boss2() = delete;

        EnemyInfo info() const override {
            return { 70000, { 1500, 60 }, 5, EnemyType::Boss2, 48};
        }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core

#endif //TOWERDEFENCE_BOSS_2_H
