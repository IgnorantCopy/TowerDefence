#ifndef TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
#define TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct AggressiveMagician final : Tower {

            TowerInfo info() const override {
                return { 450, 8, 0, 16, 3, AttackType::Magic };
            }
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
