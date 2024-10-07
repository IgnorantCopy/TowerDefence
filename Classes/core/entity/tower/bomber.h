#ifndef TOWERDEFENCE_BOMBER_H
#define TOWERDEFENCE_BOMBER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Bomber final : Tower {

            TowerInfo info() const override {
                return { 1600, 16, 0, 20, AttackType::Physics };
            }
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_BOMBER_H
