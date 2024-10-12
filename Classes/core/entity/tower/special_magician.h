#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct SpecialMagician final : Tower {
            timer::Timer release_skill_; // timer to release skill

            // store functions that will be executed after certain time
            timer::CallbackTimer<SpecialMagician &, GridRef> timeouts_;

            SpecialMagician(id::Id id, const timer::Clock &clk);

            static constexpr TowerInfo INFO { 1400, 14, 0, 30, 2, AttackType::Magic, TowerType::SpecialMagician};
            TowerInfo info() const override { return INFO; }

            int layer = 0;
            bool skill = false;

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_H
