#ifndef TOWERDEFENCE_BOMBER_PLUS_H
#define TOWERDEFENCE_BOMBER_PLUS_H

#include "../entity.h"

namespace towerdefence {
    namespace core {

        struct BomberPlus final : Tower {

            static constexpr TowerInfo INFO { 2000, 20, 0, 20, 4, AttackType::Physics };
            TowerInfo info() const override { return INFO; }

            void on_tick(GridRef g) override;
        };
    } // namespace core
} // namespace towerdefence



#endif //TOWERDEFENCE_BOMBER_PLUS_H
