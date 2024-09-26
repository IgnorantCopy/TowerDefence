#ifndef TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct diffusiveMagicianPlus final : Tower {

            TowerInfo info() const override {
                return { 2000, 21, 0, 20, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_DIFFUSIVE_MAGICIAN_PLUS_H
