#ifndef TOWERDEFENCE_BOSS_2_H
#define TOWERDEFENCE_BOSS_2_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Boss2 final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 70000, .defence = {.physics = 1500, .magic = 60}, .speed = 5,
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_BOSS_2_H
