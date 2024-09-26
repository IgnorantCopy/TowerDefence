#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct highspeedArchorPlus final : Tower {

            TowerInfo info() const override {
                return { 1300, 15, 0, 10, AttackType::Physics };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
