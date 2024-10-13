#ifndef TOWERDEFENCE_SPEED_UP_H
#define TOWERDEFENCE_SPEED_UP_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct SpeedUp final : Enemy {

            EnemyInfo info() const override {
                return { 5000, { 200, 30 }, 10, EnemyType::SpeedUp, 24};
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_SPEED_UP_H
