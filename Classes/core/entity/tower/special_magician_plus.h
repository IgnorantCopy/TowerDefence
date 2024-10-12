#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct SpecialMagicianPlus final : Tower, TimeOutMixin<SpecialMagicianPlus>{
            timer::Timer release_skill_; // timer to release skill

            // store functions that will be executed after certain time
            timer::CallbackTimer<SpecialMagicianPlus &, GridRef> timeouts_;

            SpecialMagicianPlus(id::Id id, const timer::Clock &clk);

            static constexpr TowerInfo INFO { 1800, 25, 0, 30, 2, AttackType::Magic, TowerType::SpecialMagicianPlus};
            TowerInfo info() const override { return INFO; }

            int layer = 0;
            bool skill = false;

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
