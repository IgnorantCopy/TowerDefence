#ifndef TOWERDEFENCE_AGGRESSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_AGGRESSIVE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct AggressiveMagicianPlus final : Tower {

            TowerInfo info() const override {
                return { 650, 14, 0, 16, AttackType::Magic };
            }
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_AGGRESSIVE_MAGICIAN_PLUS_H
