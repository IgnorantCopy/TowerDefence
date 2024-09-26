#ifndef TOWERDEFENCE_CORE_MAGICIAN_H
#define TOWERDEFENCE_CORE_MAGICIAN_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct coreMagician final : Tower {

            TowerInfo info() const override {
                return { 700, 12, 0, 16, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_CORE_MAGICIAN_H
