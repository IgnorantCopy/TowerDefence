#ifndef TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct decelerateMagicianPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 600, .cost = 16, .deploy_interval = 0, .attack_interval = 19, .attack_type = AttackType::Magic
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DECELERATE_MAGICIAN_PLUS_H
