#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct diffusiveMagician final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 1500, .cost = 15, .deploy_interval = 0, .attack_interval = 25, .attack_type = AttackType::Magic
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
