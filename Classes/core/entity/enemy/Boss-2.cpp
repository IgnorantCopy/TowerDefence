#include "Boss-2.h"
#include "../../map.h"

namespace towerdefence::core {

Boss2::Boss2(id::Id id, route::Route route, const timer::Clock &clk)
    : Enemy(id, route), release_skill_(clk.with_period_sec(20)) {
    this->add_buff_in({id, Buff::INVINCIBLE}, Buff::invincible(true),
                      clk.with_duration_sec(20));
}

void Boss2::on_tick(GridRef g) {
    Enemy::on_tick(g);

    if (g.clock().is_triggered(release_skill_)) {
        for (auto &grid : g.map.grids) {
            grid.with_tower([this,
                             &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                tower->add_buff_in({this->id, Buff::DEFAULT},
                                   Buff::attack_speed(-50) & Buff::silent(true),
                                   clk.with_duration_sec(10));
            });
        }
    }
}
} // namespace towerdefence::core