#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct highspeedArchor final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 2400, .cost = 18, .deploy_interval = 0, .attack_type = AttackType::Physics
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_H
