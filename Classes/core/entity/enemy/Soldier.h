#ifndef TOWERDEFENCE_SOLDIER_H
#define TOWERDEFENCE_SOLDIER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Soldier final : Enemy {

            EnemyInfo info() const override {
                return { 3200, { 50, 0 }, 10 };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence

#endif //TOWERDEFENCE_SOLDIER_H
