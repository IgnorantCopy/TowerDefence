#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct DecelerateMagicianPlus final : Tower {

            static constexpr TowerInfo INFO { 600, 16, 0, 19, 2, AttackType::Magic };
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
