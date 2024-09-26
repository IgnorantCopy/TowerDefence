#ifndef TOWERDEFENCE_HELPER_BASE_H
#define TOWERDEFENCE_HELPER_BASE_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct helperBase final : Tower {

            TowerInfo info() const override {
                return { 250, 5, 25, 18, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_HELPER_BASE_H
