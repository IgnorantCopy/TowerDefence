#ifndef TOWERDEFENCE_BOMBER_PLUS_H
#define TOWERDEFENCE_BOMBER_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct bomberPlus final : Tower {

            TowerInfo info() const override {
                return { 2000, 20, 0, 20, AttackType::Physics };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_BOMBER_PLUS_H
