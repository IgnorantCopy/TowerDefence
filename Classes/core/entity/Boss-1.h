#ifndef TOWERDEFENCE_BOSS_1_H
#define TOWERDEFENCE_BOSS_1_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct Boss_1 final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 50000, .defence = {.physics = 800, .magic = 30}, .speed = 5,
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_BOSS_1_H
