#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct specialMagician final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 1400, .cost = 14, .deploy_interval = 0, .attack_interval = 30, .attack_type = AttackType::Magic
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_H
