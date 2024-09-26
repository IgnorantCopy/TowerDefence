#ifndef TOWERDEFENCE_CRAB_H
#define TOWERDEFENCE_CRAB_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct Crab final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 3000, .defence = {.physics = 1500, .magic = 85}, .speed = 10,
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_CRAB_H
