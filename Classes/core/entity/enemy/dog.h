#ifndef TOWERDEFENCE_DOG_H
#define TOWERDEFENCE_DOG_H

#include "../entity.h"

namespace towerdefence {
namespace core {

struct Dog final : Enemy {

    Dog(id::Id id, route::Route route) : Enemy(id, route) {}

    EnemyInfo info() const override {
        return {90000000, {0, 0}, 19, EnemyType::Dog, 15};
    }
};
} // namespace core
} // namespace towerdefence

#endif // TOWERDEFENCE_DOG_H
