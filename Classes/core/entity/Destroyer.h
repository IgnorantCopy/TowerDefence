#ifndef TOWERDEFENCE_DESTROYER_H
#define TOWERDEFENCE_DESTROYER_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct Destroyer final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 10000, .defence = {.physics = 500, .magic = 20}, .speed = 5,
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_DESTROYER_H
