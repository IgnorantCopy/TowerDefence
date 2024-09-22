#ifndef TOWERDEFENCE_NOT_ATTACKED_H
#define TOWERDEFENCE_NOT_ATTACKED_H

#include "entity.h"

namespace towerdefence {
    namespace core {

        struct NotAttacked final : Enemy {

            EnemyInfo info() const override {
                return {
                        .health = 16000, .defence = {.physics = 500, .magic = 20}, .speed = 8,
                };
            }

            void update(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_NOT_ATTACKED_H
