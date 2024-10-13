#ifndef TOWERDEFENCE_HELPER_BASE_H
#define TOWERDEFENCE_HELPER_BASE_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct HelperBase final : Tower {
        timer::Timer release_skill_;

        HelperBase(id::Id id, const timer::Clock & clk);
        HelperBase() = delete;

        static constexpr TowerInfo INFO { 250, 5, 25, 18, 2, AttackType::Magic, TowerType::HelperBase};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_HELPER_BASE_H
