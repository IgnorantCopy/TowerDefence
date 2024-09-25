#ifndef TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_CORE_MAGICIAN_PLUS_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct coreMagicianPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 1100, .cost = 16, .deploy_interval = 0, .attack_interval = 16, .attack_type = AttackType::Magic
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
