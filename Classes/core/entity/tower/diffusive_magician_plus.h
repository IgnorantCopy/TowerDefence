#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct DiffusiveMagicianPlus final : Tower {

            TowerInfo info() const override {
                return { 2000, 21, 0, 20, 4, AttackType::Magic };
            }
       };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
