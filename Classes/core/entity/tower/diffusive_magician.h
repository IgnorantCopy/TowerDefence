#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct diffusiveMagician final : Tower {

            TowerInfo info() const override {
                return { 1500, 15, 0, 25, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_H
