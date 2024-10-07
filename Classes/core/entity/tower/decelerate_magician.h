#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct DecelerateMagician final : Tower {

            TowerInfo info() const override {
                return { 600, 16, 0, 19, 2, AttackType::Magic };
            }
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_H
