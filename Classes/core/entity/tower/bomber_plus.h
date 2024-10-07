#ifndef TOWERDEFENCE_BOMBER_PLUS_H
#define TOWERDEFENCE_BOMBER_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct BomberPlus final : Tower {

            TowerInfo info() const override {
                return { 2000, 20, 0, 20, 4, AttackType::Physics };
            }
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_BOMBER_PLUS_H
