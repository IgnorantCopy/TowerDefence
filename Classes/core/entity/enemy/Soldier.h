#ifndef TOWERDEFENCE_SOLDIER_H
#define TOWERDEFENCE_SOLDIER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Soldier final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 3200, .defence = {.physics = 50, .magic = 0}, .speed = 10,
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence

#endif //TOWERDEFENCE_SOLDIER_H
