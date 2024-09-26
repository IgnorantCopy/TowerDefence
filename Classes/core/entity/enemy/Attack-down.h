#ifndef TOWERDEFENCE_ATTACK_DOWN_H
#define TOWERDEFENCE_ATTACK_DOWN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct AttackDown final : Enemy {

            EnemyInfo info() const override {
                return { 20000, { 700, 50 }, 8 };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_ATTACK_DOWN_H
