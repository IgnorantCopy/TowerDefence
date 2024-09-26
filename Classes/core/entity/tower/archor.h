#ifndef TOWERDEFENCE_ARCHOR_H
#define TOWERDEFENCE_ARCHOR_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct archor final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 2400, .cost = 18, .deploy_interval = 0, .attack_interval = 10, .attack_type = AttackType::Physics
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_ARCHOR_H
