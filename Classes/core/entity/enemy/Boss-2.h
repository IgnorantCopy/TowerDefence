#ifndef TOWERDEFENCE_BOSS_2_H
#define TOWERDEFENCE_BOSS_2_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Boss2 final : Enemy {

            EnemyInfo info() const override {
                return { 70000, { 1500, 60 }, 5 };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_BOSS_2_H
