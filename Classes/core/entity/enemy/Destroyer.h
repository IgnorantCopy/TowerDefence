#ifndef TOWERDEFENCE_DESTROYER_H
#define TOWERDEFENCE_DESTROYER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Destroyer final : Enemy {

            Destroyer(id::Id id, route::Route route): Enemy(id, route) {}

            EnemyInfo info() const override {
                return { 10000, { 500, 20 }, 5, EnemyType::Destroyer, 32};
            }
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_DESTROYER_H
