#ifndef TOWERDEFENCE_WEAKEN_MAGICIAN_H
#define TOWERDEFENCE_WEAKEN_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct WeakenMagician final : Tower {
            timer::Timer release_skill_; // timer to release skill

            // store functions that will be executed after certain time
            timer::CallbackTimer<WeakenMagician &, GridRef> timeouts_;

            WeakenMagician(id::Id id, const timer::Clock &clk);

            static constexpr TowerInfo INFO { 500, 13, 0, 16, 3, AttackType::Magic, TowerType::WeakenMagician};
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_WEAKEN_MAGICIAN_H
