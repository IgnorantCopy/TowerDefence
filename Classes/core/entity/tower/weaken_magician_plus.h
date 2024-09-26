#ifndef TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H
#define TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct weakenMagicianPlus final : Tower {

            TowerInfo info() const override {
                return { 700, 18, 0, 16, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_WEAKEN_MAGICIAN_PLUS_H
