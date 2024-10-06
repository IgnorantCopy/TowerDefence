#ifndef TOWERDEFENCE_HELPER_BASE_H
#define TOWERDEFENCE_HELPER_BASE_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct helperBase final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        helperBase(id::Id id, const timer::Clock & clk);
        helperBase() = delete;

        TowerInfo info() const override { return { 250, 5, 25, 18, AttackType::Magic }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_HELPER_BASE_H
