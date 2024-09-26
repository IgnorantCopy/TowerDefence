#ifndef TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
#define TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct aggressiveMagician final : Tower {

            TowerInfo info() const override {
                return { 450, 8, 0, 16, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_AGGRESSIVE_MAGICIAN_H
