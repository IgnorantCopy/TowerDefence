#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct SpecialMagician final : Tower {

            static constexpr TowerInfo INFO { 1400, 14, 0, 30, 2, AttackType::Magic, TowerType::SpecialMagician};
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_H
