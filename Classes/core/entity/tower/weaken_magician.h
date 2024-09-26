#ifndef TOWERDEFENCE_WEAKEN_MAGICIAN_H
#define TOWERDEFENCE_WEAKEN_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct weakenMagician final : Tower {

            TowerInfo info() const override {
                return { 500, 13, 0, 16, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_WEAKEN_MAGICIAN_H
