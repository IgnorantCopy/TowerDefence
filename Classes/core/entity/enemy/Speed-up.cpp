#include "Speed-up.h"
#include "../../map.h"

namespace towerdefence {
namespace core {

void SpeedUp::on_tick(GridRef g) {
    Enemy::on_tick(g);

    for (auto grid : g.with_radius(1, linf_dis)) {
        grid.grid.with_enemy([this, &clk = g.clock()](Enemy &enemy) {
            enemy.add_buff_in({this->id, Buff::DEFAULT}, Buff::speed(0.5),
                              clk.with_duration_sec(5));
        });
    }
}

} // namespace core
} // namespace towerdefence