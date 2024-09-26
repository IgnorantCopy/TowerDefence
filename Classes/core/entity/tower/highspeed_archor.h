#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct highspeedArchor final : Tower {

            TowerInfo info() const override {
                return { 2400, 18, 0, 10, AttackType::Physics };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_H
