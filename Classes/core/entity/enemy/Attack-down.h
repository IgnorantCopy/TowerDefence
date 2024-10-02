#ifndef TOWERDEFENCE_ATTACK_DOWN_H
#define TOWERDEFENCE_ATTACK_DOWN_H

#include "../entity.h"

namespace towerdefence::core {

struct Map;

struct AttackDown final : Enemy {
    timer::Timer release_skill_;

    AttackDown(id::Id id, const timer::Clock & clk);
    AttackDown() = delete;

    EnemyInfo info() const override { return {20000, {700, 50}, 8}; }

    void on_tick(GridRef g) override;
    void on_death(GridRef g) override;
};
} // namespace towerdefence::core

#endif // TOWERDEFENCE_ATTACK_DOWN_H
