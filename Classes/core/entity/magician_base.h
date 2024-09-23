#ifndef TOWERDEFENCE_MAGICIAN_BASE_H
#define TOWERDEFENCE_MAGICIAN_BASE_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct magicianBase final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 450, .cost = 8, .deploy_interval = 0, .attack_interval = 18, .attack_type = AttackType::Magic
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_MAGICIAN_BASE_H
