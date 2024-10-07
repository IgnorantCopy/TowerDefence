#ifndef TOWERDEFENCE_NOT_ATTACKED_H
#define TOWERDEFENCE_NOT_ATTACKED_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct NotAttacked final : Enemy {
            bool not_hit_ = true;

            EnemyInfo info() const override {
                return { 16000, { 500, 20 }, 8 };
            }
            
            void on_hit(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_NOT_ATTACKED_H
