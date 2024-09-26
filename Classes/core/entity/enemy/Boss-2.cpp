#include "Boss-2.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        void Boss2::on_tick(GridRef g) {
            for (auto &grid : g.map.grids) {
                grid.with_tower([this](std::unique_ptr<Tower> &tower) {
                    tower->add_buff(this->id, Buff::attack_speed(-50) & Buff::silent(true));
                });
            }
        }

    } // namespace core
} // namespace towerdefence