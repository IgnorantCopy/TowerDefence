#ifndef TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
#define TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct specialMagicianPlus final : Tower {

            TowerInfo info() const override {
                return { 1800, 25, 0, 30, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_SPECIAL_MAGICIAN_PLUS_H
