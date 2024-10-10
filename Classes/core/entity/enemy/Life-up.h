#ifndef TOWERDEFENCE_LIFE_UP_H
#define TOWERDEFENCE_LIFE_UP_H

#include "../entity.h"
#include "../route.h"

namespace towerdefence {
namespace core {

struct LifeUp final : Enemy {
    timer::Timer release_skill_;

    LifeUp(id::Id id, route::Route route, const timer::Clock &clk)
        : Enemy(id, route), release_skill_(clk.with_period_sec(15)) {}

    EnemyInfo info() const override { return {6000, {130, 30}, 8}; }

    void on_tick(GridRef g) override;
};
} // namespace core
} // namespace towerdefence

#endif // TOWERDEFENCE_LIFE_UP_H
