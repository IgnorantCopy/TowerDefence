#ifndef TOWERDEFENCE_AGGRESSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_AGGRESSIVE_MAGICIAN_PLUS_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct aggressiveMagicianPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 650, .cost = 14, .deploy_interval = 0, .attack_interval = 16, .attack_type = AttackType::Magic
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_AGGRESSIVE_MAGICIAN_PLUS_H
