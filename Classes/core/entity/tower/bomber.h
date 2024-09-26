#ifndef TOWERDEFENCE_BOMBER_H
#define TOWERDEFENCE_BOMBER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct bomber final : Tower {

            TowerInfo info() const override {
                return {
                        .attack = 1600, .cost = 16, .deploy_interval = 0, .attack_interval = 20, .attack_type = AttackType::Physics
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_BOMBER_H
