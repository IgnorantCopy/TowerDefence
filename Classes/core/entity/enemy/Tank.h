#ifndef TOWERDEFENCE_TANK_H
#define TOWERDEFENCE_TANK_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Tank final : Enemy {

            EnemyInfo info() const override {
                return { 10000, { 300, 40 }, 5 };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_TANK_H
