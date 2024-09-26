#include "Attack-down.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        void AttackDown::on_tick(GridRef g) {
            for (auto grid : g.with_radix(2, l1_dis)) {
                grid.grid.with_tower([this](std::unique_ptr<Tower> &tower) {
                    tower->add_buff(this->id, {.attack = -0.5});
                });
            }
        }

    } // namespace core
} // namespace towerdefence