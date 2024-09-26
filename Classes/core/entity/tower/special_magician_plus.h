#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct specialMagicianPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 1800, .cost = 25, .deploy_interval = 0, .attack_interval = 30, .attack_type = AttackType::Magic
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
