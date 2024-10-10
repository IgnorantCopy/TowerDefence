#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct DecelerateMagicianPlus final : Tower {

            TowerInfo info() const override {
                return { 600, 16, 0, 19, 2, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
