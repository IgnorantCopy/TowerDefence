#ifndef TOWERDEFENCE_MAGICIAN_BASE_H
#define TOWERDEFENCE_MAGICIAN_BASE_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct magicianBase final : Tower {

            TowerInfo info() const override {
                return { 450, 8, 0, 18, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_MAGICIAN_BASE_H
