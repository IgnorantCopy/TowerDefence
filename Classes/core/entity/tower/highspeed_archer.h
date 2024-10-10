#ifndef TOWERDEFENCE_HIGHSPEED_ARCHER_H
#define TOWERDEFENCE_HIGHSPEED_ARCHER_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct HighspeedArcher final : Tower {

        TowerInfo info() const override { return { 1000, 10, 0, 10, 2, AttackType::Physics }; }

    };
} // namespace towerdefence::core



#endif //TOWERDEFENCE_HIGHSPEED_ARCHER_H
