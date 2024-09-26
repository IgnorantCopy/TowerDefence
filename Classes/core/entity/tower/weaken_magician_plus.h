#ifndef TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H
#define TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct weakenMagicianPlus final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 700, .cost = 18, .deploy_interval = 0, .attack_interval = 16, .attack_type = AttackType::Magic
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H
