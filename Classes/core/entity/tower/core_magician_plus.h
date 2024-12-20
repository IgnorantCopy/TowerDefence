#ifndef TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_CORE_MAGICIAN_PLUS_H

#include "../../id.h"
#include "../entity.h"

namespace towerdefence::core {
    struct CoreMagicianPlus final : Tower, TimeOutMixin<CoreMagicianPlus> {
        timer::Timer release_skill_;

        bool skill_ = false;

        CoreMagicianPlus(id::Id id, const timer::Clock & clk);
        CoreMagicianPlus() = delete;

        static constexpr TowerInfo INFO { 1500, 16, 0, 16, 2, AttackType::Magic, TowerType::CoreMagicianPlus};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
