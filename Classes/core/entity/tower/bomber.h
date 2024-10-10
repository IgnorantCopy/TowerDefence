#ifndef TOWERDEFENCE_BOMBER_H
#define TOWERDEFENCE_BOMBER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Bomber final : Tower {

            TowerInfo info() const override {
                return { 1600, 16, 0, 20, 4, AttackType::Physics };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_BOMBER_H
