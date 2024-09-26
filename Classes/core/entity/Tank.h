#ifndef TOWERDEFENCE_TANK_H
#define TOWERDEFENCE_TANK_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct Tank final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 10000, .defence = {.physics = 300, .magic = 40}, .speed = 5,
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_TANK_H
