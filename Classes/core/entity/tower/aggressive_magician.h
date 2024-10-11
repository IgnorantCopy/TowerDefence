#ifndef TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
#define TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct AggressiveMagician final : Tower {

            static constexpr TowerInfo INFO { 450, 8, 0, 16, 3, AttackType::Magic };
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
