#ifndef TOWERDEFENCE_BOSS_1_H
#define TOWERDEFENCE_BOSS_1_H

#include "../../id.h"
#include "../entity.h"
#include "../route.h"
#include <unordered_set>

namespace towerdefence::core {
    struct Boss1 final : Enemy {
        struct {
            timer::Timer dec_atk_spd;
            timer::Timer silent;
            timer::Timer withdraw;
        } release_skill_;
        std::unordered_set<id::Id> has_buff_;

        Boss1(id::Id id, route::Route route, const timer::Clock & clk);

        EnemyInfo info() const override {
            return { 50000, { 800, 30 }, 5 };
        }

        void on_tick(GridRef g) override;
        void on_death(GridRef g) override;
    };
} // namespace towerdefence::core

#endif //TOWERDEFENCE_BOSS_1_H
