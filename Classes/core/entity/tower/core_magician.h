#ifndef TOWERDEFENCE_CORE_MAGICIAN_H
#define TOWERDEFENCE_CORE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"

namespace towerdefence::core {
    struct CoreMagician final : Tower {
        timer::Timer release_skill_;

        CoreMagician(id::Id id, const timer::Clock & clk);
        CoreMagician() = delete;

        static constexpr TowerInfo INFO { 700, 12, 0, 16, 2, AttackType::Magic, TowerType::CoreMagician};
        TowerInfo info() const override { return INFO; }

        bool skill = false;

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_CORE_MAGICIAN_H
