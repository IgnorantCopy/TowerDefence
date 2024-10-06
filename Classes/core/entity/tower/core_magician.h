#ifndef TOWERDEFENCE_CORE_MAGICIAN_H
#define TOWERDEFENCE_CORE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct coreMagician final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        coreMagician(id::Id id, const timer::Clock & clk);
        coreMagician() = delete;

        TowerInfo info() const override { return { 700, 12, 0, 16, AttackType::Magic }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_CORE_MAGICIAN_H
