#ifndef TOWERDEFENCE_WARLOCK_H
#define TOWERDEFENCE_WARLOCK_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Warlock final : Enemy {

            Warlock(id::Id id, route::Route route): Enemy(id, route) {}

            EnemyInfo info() const override {
                return { 4000, { 150, 20 }, 10, EnemyType::Warlock, 41};
            }
        };
    } // namespace core
} // namespace towerdefence

#endif //TOWERDEFENCE_WARLOCK_H
