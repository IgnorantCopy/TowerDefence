#include "Boss-1.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        void Boss1::on_tick(GridRef g) {
            for (auto &grid : g.map.grids) {
                grid.with_tower([this](std::unique_ptr<Tower> &tower) {
                    tower->add_buff(this->id, {.attack_speed = -30, .silent = true});
                });
            }
        }

    } // namespace core
} // namespace towerdefence