#ifndef TOWERDEFENCE_BOMBER_PLUS_H
#define TOWERDEFENCE_BOMBER_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
struct BomberPlus final : Tower {
    timer::Timer release_skill_; // timer to release skill
    timer::Timer attack_;        // timer to do normal attack

    // store functions that will be executed after certain time
    timer::CallbackTimer<BomberPlus &, GridRef> timeouts_;

    BomberPlus(id::Id id, const timer::Clock &clk);

    static constexpr TowerInfo INFO{2000, 20, 0, 20, 4, AttackType::Physics};
    TowerInfo info() const override { return INFO; }
    void on_tick(GridRef g) override;
};
} // namespace towerdefence::core

#endif