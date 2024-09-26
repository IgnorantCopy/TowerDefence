#ifndef TOWERDEFENCE_HELPER_BASE_H
#define TOWERDEFENCE_HELPER_BASE_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct helperBase final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 250, .cost = 5, .deploy_interval = 25, .attack_interval = 18, .attack_type = AttackType::Magic
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_HELPER_BASE_H
