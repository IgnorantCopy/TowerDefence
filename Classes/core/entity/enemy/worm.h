#ifndef TOWERDEFENCE_CORE_ENTITY_WORM
#define TOWERDEFENCE_CORE_ENTITY_WORM

#include "../entity.h"
#include "../route.h"

namespace towerdefence {
namespace core {

struct Worm final : Enemy {
    Worm(id::Id id, route::Route route) : Enemy(id, route) {}

    EnemyInfo info() const override { return {1000, {0, 0}, 10, EnemyType::Worm, 30}; }

    void on_tick(GridRef g) override;
    void on_death(GridRef g) override;
};
} // namespace core
} // namespace towerdefence

#endif