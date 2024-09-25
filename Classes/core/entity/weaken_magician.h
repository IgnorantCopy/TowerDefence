#ifndef TOWERDEFENCE_WEAKEN_MAGICIAN_H
#define TOWERDEFENCE_WEAKEN_MAGICIAN_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct weakenMagician final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 500, .cost = 13, .deploy_interval = 0, .attack_interval = 16, .attack_type = AttackType::Magic
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_WEAKEN_MAGICIAN_H
