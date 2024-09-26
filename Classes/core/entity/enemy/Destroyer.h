#ifndef TOWERDEFENCE_DESTROYER_H
#define TOWERDEFENCE_DESTROYER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Destroyer final : Enemy {

            EnemyInfo info() const override {
                return { 10000, { 500, 20 }, 5 };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_DESTROYER_H
