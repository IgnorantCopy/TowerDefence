#ifndef TOWERDEFENCE_CRAB_H
#define TOWERDEFENCE_CRAB_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Crab final : Enemy {

            Crab(id::Id id, route::Route route) : Enemy(id, route) {}

            EnemyInfo info() const override {
                return { 3000, { 1500, 85 }, 10, EnemyType::Crab, 30};
            }
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_CRAB_H
