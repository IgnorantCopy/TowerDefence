#ifndef TOWERDEFENCE_MAGICIAN_BASE_H
#define TOWERDEFENCE_MAGICIAN_BASE_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct MagicianBase final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        MagicianBase(id::Id id, const timer::Clock & clk);
        MagicianBase() = delete;

        static constexpr TowerInfo INFO { 400, 9, 15, 18, 2, AttackType::Magic };
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_MAGICIAN_BASE_H
