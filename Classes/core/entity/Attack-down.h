#ifndef TOWERDEFENCE_ATTACK_DOWN_H
#define TOWERDEFENCE_ATTACK_DOWN_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct Attack_down final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 20000, .defence = {.physics = 700, .magic = 50}, .speed = 8,
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_ATTACK_DOWN_H
