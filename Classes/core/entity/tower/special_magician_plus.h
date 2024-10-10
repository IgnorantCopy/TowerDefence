#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct SpecialMagicianPlus final : Tower {

            static constexpr TowerInfo INFO { 1800, 25, 0, 30, 2, AttackType::Magic };
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
