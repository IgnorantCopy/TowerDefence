#ifndef TOWERDEFENCE_CORE_ENTITY_WORM
#define TOWERDEFENCE_CORE_ENTITY_WORM

#include "entity.h"

namespace towerdefence {
namespace core {

struct Worm final : Enemy {

    EnemyInfo info() const override {
        return {
            .health = 1000, .defence = {.physics = 0, .magic = 0}, .speed = 10,
        };
    }

    void update(GridRef g) override;
};
} // namespace core
} // namespace towerdefence

#endif