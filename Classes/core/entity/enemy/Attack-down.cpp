#include "Attack-down.h"
#include "../../map.h"
#include <stdexcept>

namespace towerdefence {
namespace core {

AttackDown::AttackDown(id::Id id, route::Route route, const timer::Clock &clk) : Enemy(id, route), release_skill_(clk.with_period_sec(20)) {}

void AttackDown::on_tick(GridRef g) {
    this->update_buff(g.clock());

    if (g.clock().is_triggered(release_skill_)) {
        this->move_ = g.clock().never();
        for (auto grid : g.with_radius(2, l1_dis)) {
            grid.grid.with_tower(
                [this, &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                    tower->add_buff_in({this->id, Buff::DEFAULT},
                                       Buff::attack(-0.5),
                                       clk.with_duration_sec(25));
                    has_buff_.insert(tower->id);
                });
        }
        g.on_enemy_release_skill(*this, g.map, 25);
        this->timeouts_.add_callback(
        g.clock().with_duration(68), [](AttackDown &self, GridRef g) {
            self.reset_move_timer(g.clock());

            return false;

            });
    }
}

void AttackDown::on_death(GridRef g) {
    Enemy::on_death(g);
    for (auto tower_id : has_buff_) {
        try {
            auto &tower = g.map.get_tower_by_id(tower_id);
            tower.remove_buff_from(id);
        } catch (const std::out_of_range &) {
        }
    }
}

} // namespace core
} // namespace towerdefence