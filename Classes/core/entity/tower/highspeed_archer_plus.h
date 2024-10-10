#ifndef TOWERDEFENCE_HIGHSPEED_ARCHER_PLUS_H
#define TOWERDEFENCE_HIGHSPEED_ARCHER_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct HighspeedArcherPlus final : Tower {

        TowerInfo info() const override { return { 1300, 15, 0, 10, 2, AttackType::Physics }; }

    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_HIGHSPEED_ARCHER_PLUS_H
