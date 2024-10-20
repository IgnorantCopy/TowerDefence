#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H

#include "../../id.h"
#include "../entity.h"

namespace towerdefence::core {
    struct DecelerateMagicianPlus final : Tower, TimeOutMixin<DecelerateMagicianPlus>  {
        timer::Timer release_skill_;

        DecelerateMagicianPlus(id::Id id, const timer::Clock & clk);
        DecelerateMagicianPlus() = delete;

        static constexpr TowerInfo INFO { 600, 16, 0, 19, 2, AttackType::Magic, TowerType::DecelerateMagicianPlus};
        TowerInfo info() const override { return INFO; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
