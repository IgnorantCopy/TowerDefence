#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct highspeedArchorPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 1300, .cost = 15, .deploy_interval = 0, .attack_interval = 10, .attack_type = AttackType::Physics
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
