#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct DiffusiveMagician final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        DiffusiveMagician(id::Id id, const timer::Clock & clk);
        DiffusiveMagician() = delete;

        TowerInfo info() const override { return { 1500, 15, 0, 25, AttackType::Magic }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
