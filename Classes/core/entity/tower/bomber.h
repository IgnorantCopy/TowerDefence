#ifndef TOWERDEFENCE_BOMBER_H
#define TOWERDEFENCE_BOMBER_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct Bomber final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        Bomber(id::Id id, const timer::Clock & clk);
        Bomber() = delete;

        TowerInfo info() const override { return { 1600, 16, 0, 20, 4, AttackType::Physics }; }

        bool skill = false;

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_BOMBER_H
