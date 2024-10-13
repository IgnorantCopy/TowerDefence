#ifndef TOWERDEFENCE_SOLDIER_H
#define TOWERDEFENCE_SOLDIER_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Soldier final : Enemy {

            EnemyInfo info() const override {
                return { 3200, { 50, 0 }, 10, EnemyType::Soldier, 24};
            }
        };
    } // namespace core
} // namespace towerdefence

#endif //TOWERDEFENCE_SOLDIER_H
