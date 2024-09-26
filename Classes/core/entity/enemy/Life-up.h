#ifndef TOWERDEFENCE_LIFE_UP_H
#define TOWERDEFENCE_LIFE_UP_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct LifeUp final : Enemy {

            EnemyInfo info() const override {
                return { 6000, { 130, 30 }, 8 };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_LIFE_UP_H
