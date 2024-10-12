#ifndef TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
#define TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H

#include "../entity.h"
#include <unordered_set>

namespace towerdefence {
    namespace core {

        struct AggressiveMagician final : Tower {
            timer::Timer release_skill_; // timer to release skill
            timer::Timer attack_;        // timer to do normal attack

            std::unordered_set<id::Id> has_buff_;

            AggressiveMagician(id::Id id, const timer::Clock &clk);

            static constexpr TowerInfo INFO { 450, 8, 0, 16, 3, AttackType::Magic, TowerType::AggressiveMagician};
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
            void on_death(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
