#ifndef TOWERDEFENCE_CORE_MAGICIAN_H
#define TOWERDEFENCE_CORE_MAGICIAN_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct CoreMagician final : Tower {

        TowerInfo info() const override { return { 700, 12, 0, 16, 2, AttackType::Magic }; }

    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_CORE_MAGICIAN_H
