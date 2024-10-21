#include "Boss-1.h"
#include "../../map.h"
#include "Boss-2.h"

namespace towerdefence {
namespace core {

Boss1::Boss1(id::Id id, route::Route route, const timer::Clock &clk)
    : Enemy(id, route),
      release_skill_(clk.with_period_sec(20)) {}

void Boss1::on_tick(GridRef g) {
    Enemy::on_tick(g);

    this->timeouts_.on_tick(g.clock(), *this, g);

    if (g.clock().is_triggered(release_skill_)) {
        this->move_ = g.clock().never();
        for (auto &grid : g.map.grids) {
            grid.with_tower([this,
                                    &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                tower->add_buff_in({this->id, Buff::DEFAULT},
                                   Buff::attack_speed(-50) & Buff::silent(true),
                                   clk.with_duration_sec(10));
            });
        }
        g.on_enemy_release_skill(*this, g.map, 10);
        this->timeouts_.add_callback(
            g.clock().with_duration(135), [](Boss1 &self, GridRef g) {
                self.reset_move_timer(g.clock());

                return false;

            });
    }
}

} // namespace core
} // namespace towerdefence
