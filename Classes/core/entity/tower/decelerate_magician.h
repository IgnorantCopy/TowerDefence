#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct DecelerateMagician final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        DecelerateMagician(id::Id id, const timer::Clock & clk);
        DecelerateMagician() = delete;

        static constexpr TowerInfo INFO { 600, 16, 0, 19, 2, AttackType::Magic, TowerType::DecelerateMagician};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_H
