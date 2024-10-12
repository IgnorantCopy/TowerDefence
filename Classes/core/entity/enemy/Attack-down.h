#ifndef TOWERDEFENCE_ATTACK_DOWN_H
#define TOWERDEFENCE_ATTACK_DOWN_H

#include "../../id.h"
#include "../entity.h"
#include "../route.h"
#include <unordered_set>

namespace towerdefence::core {
struct AttackDown final : Enemy {
    timer::Timer release_skill_;
    std::unordered_set<id::Id> has_buff_;

    AttackDown(id::Id id, route::Route route, const timer::Clock & clk);
    AttackDown() = delete;

    EnemyInfo info() const override { return {20000, {700, 50}, 8, EnemyType::AttackDown}; }

    void on_tick(GridRef g) override;
    void on_death(GridRef g) override;
};
} // namespace towerdefence::core

#endif // TOWERDEFENCE_ATTACK_DOWN_H
