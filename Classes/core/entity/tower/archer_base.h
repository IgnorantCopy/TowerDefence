#ifndef TOWERDEFENCE_ARCHER_BASE_H
#define TOWERDEFENCE_ARCHER_BASE_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct archorBase final : Tower {

            TowerInfo info() const override {
                return { 500, 7, 10, 15, AttackType::Physics };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_ARCHER_BASE_H
