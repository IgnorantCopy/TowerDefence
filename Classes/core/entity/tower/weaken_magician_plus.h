#ifndef TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H
#define TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct WeakenMagicianPlus final : Tower {
            timer::Timer release_skill_; // timer to release skill

            WeakenMagicianPlus(id::Id id, const timer::Clock &clk);

            static constexpr TowerInfo INFO { 700, 18, 0, 16, 3, AttackType::Magic, TowerType::WeakenMagicianPlus};
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H
