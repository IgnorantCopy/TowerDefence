#ifndef TOWERDEFENCE_ARCHER_PLUS_H
#define TOWERDEFENCE_ARCHER_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct archerPlus final : Tower {

            TowerInfo info() const override {
                return { 2800, 24, 0, 10, AttackType::Physics };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_ARCHER_PLUS_H
