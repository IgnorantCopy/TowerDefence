#ifndef TOWERDEFENCE_DOG_H
#define TOWERDEFENCE_DOG_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct Dog final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 900, .defence = {.physics = 0, .magic = 0}, .speed = 19,
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_DOG_H
