#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct decelerateMagician final : Tower {

            TowerInfo info() const override {
                return { 600, 16, 0, 19, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_H
