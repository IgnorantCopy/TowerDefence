#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct SpecialMagician final : Tower {

            TowerInfo info() const override {
                return { 1400, 14, 0, 30, 2, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_H
