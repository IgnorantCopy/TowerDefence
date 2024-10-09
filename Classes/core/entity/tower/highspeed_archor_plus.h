#ifndef TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
#define TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct HighspeedArcherPlus final : Tower {

        TowerInfo info() const override { return { 1300, 15, 0, 10, 2, AttackType::Physics }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_HIGHSPEED_ARCHOR_PLUS_H
