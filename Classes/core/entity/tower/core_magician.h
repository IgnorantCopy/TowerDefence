#ifndef TOWERDEFENCE_CORE_MAGICIAN_H
#define TOWERDEFENCE_CORE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct coreMagician final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 700, .cost = 12, .deploy_interval = 0, .attack_interval = 16, .attack_type = AttackType::Magic
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_CORE_MAGICIAN_H
