#ifndef TOWERDEFENCE_MAGICIAN_BASE_H
#define TOWERDEFENCE_MAGICIAN_BASE_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {

    struct Map;

    struct magicianBase final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        magicianBase(id::Id id, const timer::Clock & clk);
        magicianBase() = delete;

        TowerInfo info() const override { return { 400, 9, 0, 15, AttackType::Magic }; }

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_MAGICIAN_BASE_H
