#ifndef TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
#define TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct aggressiveMagician final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 450, .cost = 8, .deploy_interval = 0, .attack_interval = 16, .attack_type = AttackType::Magic
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
