#ifndef TOWERDEFENCE_WEAKEN_MAGICIAN_H
#define TOWERDEFENCE_WEAKEN_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct WeakenMagician final : Tower {

            TowerInfo info() const override {
                return { 500, 13, 0, 16, 3, AttackType::Magic };
            }
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_WEAKEN_MAGICIAN_H
