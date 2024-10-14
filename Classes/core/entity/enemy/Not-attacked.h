#ifndef TOWERDEFENCE_NOT_ATTACKED_H
#define TOWERDEFENCE_NOT_ATTACKED_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct NotAttacked final : Enemy {
            bool not_hit_ = true;

            timer::Timer release_skill_;

            NotAttacked(id::Id id, route::Route route);
            NotAttacked() = delete;

            EnemyInfo info() const override {
                return { 16000, { 500, 20 }, 8, EnemyType::NotAttacked, 36};
            }
            
            void on_hit(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence


#endif //TOWERDEFENCE_NOT_ATTACKED_H
