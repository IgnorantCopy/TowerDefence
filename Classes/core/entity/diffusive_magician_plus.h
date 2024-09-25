#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct diffusiveMagicianPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 2000, .cost = 21, .deploy_interval = 0, .attack_interval = 20, .attack_type = AttackType::Magic
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
