#include "Speed-up.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        void SpeedUp::on_tick(GridRef g) {
            for (auto grid : g.with_radix(1, linf_dis)) {
                grid.grid.with_enemy([this](Enemy &enemy) {
                    enemy.add_buff(this->id, Buff::speed(0.5));
                });
            }
        }

    } // namespace core
} // namespace towerdefence