#ifndef TOWERDEFENCE_BOSS_1_H
#define TOWERDEFENCE_BOSS_1_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Boss1 final : Enemy {

            EnemyInfo info() const override {
                return { 50000, { 800, 30 }, 5 };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_BOSS_1_H
