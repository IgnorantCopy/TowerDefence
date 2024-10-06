#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct diffusiveMagician final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        diffusiveMagician(id::Id id, const timer::Clock & clk);
        diffusiveMagician() = delete;

        TowerInfo info() const override { return { 1500, 15, 0, 25, AttackType::Magic }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
