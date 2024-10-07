#ifndef TOWERDEFENCE_CORE_MAGICIAN_H
#define TOWERDEFENCE_CORE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct CoreMagician final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        CoreMagician(id::Id id, const timer::Clock & clk);
        CoreMagician() = delete;

        TowerInfo info() const override { return { 700, 12, 0, 16, 2, AttackType::Magic }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_CORE_MAGICIAN_H
