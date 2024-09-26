#ifndef TOWERDEFENCE_WARLOCK_H
#define TOWERDEFENCE_WARLOCK_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct Warlock final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 4000, .defence = {.physics = 150, .magic = 20}, .speed = 10,
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence

#endif //TOWERDEFENCE_WARLOCK_H
