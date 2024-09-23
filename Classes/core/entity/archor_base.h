#ifndef TOWERDEFENCE_ARCHOR_BASE_H
#define TOWERDEFENCE_ARCHOR_BASE_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct archorBase final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 500, .cost = 7, .deploy_interval = 10, .attack_interval = 15, .attack_type = AttackType::Physics
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_ARCHOR_BASE_H
