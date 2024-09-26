#ifndef TOWERDEFENCE_SPEED_UP_H
#define TOWERDEFENCE_SPEED_UP_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct SpeedUp final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 5000, .defence = {.physics = 200, .magic = 30}, .speed = 10,
                };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_SPEED_UP_H
