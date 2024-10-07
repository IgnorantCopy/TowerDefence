#ifndef TOWERDEFENCE_DOG_H
#define TOWERDEFENCE_DOG_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct Dog final : Enemy {

            EnemyInfo info() const override {
                return { 900, { 0, 0 }, 19 };
            }
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_DOG_H
