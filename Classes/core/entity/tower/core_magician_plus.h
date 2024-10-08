#ifndef TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_CORE_MAGICIAN_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct CoreMagicianPlus final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        CoreMagicianPlus(id::Id id, const timer::Clock & clk);
        CoreMagicianPlus() = delete;

        TowerInfo info() const override { return { 1100, 16, 0, 16, 2, AttackType::Magic }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
