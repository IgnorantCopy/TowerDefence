#ifndef TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
#define TOWERDEFENCE_CORE_MAGICIAN_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct coreMagicianPlus final : Tower {

            TowerInfo info() const override {
                return { 1100, 16, 0, 16, AttackType::Magic };
            }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_CORE_MAGICIAN_PLUS_H
