#ifndef TOWERDEFENCE_BOMBER_PLUS_H
#define TOWERDEFENCE_BOMBER_PLUS_H

#include "../../id.h"
#include "../entity.h"
#include <unordered_set>

namespace towerdefence::core {
    struct BomberPlus final : Tower {
        timer::Timer release_skill_;
        std::unordered_set<id::Id> has_buff_;

        BomberPlus(id::Id id, const timer::Clock & clk);
        BomberPlus() = delete;

        static constexpr TowerInfo INFO { 2000, 20, 0, 20, 4, AttackType::Physics };
        TowerInfo info() const override { return INFO; }

        bool skill = false;

        void on_tick(GridRef g) override;
    };
} // namespace towerdefence::core




#endif //TOWERDEFENCE_BOMBER_PLUS_H
