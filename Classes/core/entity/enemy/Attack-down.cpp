#include "Attack-down.h"
#include "../../map.h"

namespace towerdefence {
namespace core {

AttackDown::AttackDown(Map &m)
    : release_skill_(m.clock().with_period_sec(20)) {}

void AttackDown::on_tick(GridRef g) {
    this->update_buff(g.clock());

    if (g.clock().is_triggered(release_skill_)) {
        for (auto grid : g.with_radius(2, l1_dis)) {
            grid.grid.with_tower(
                [this, &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                    tower->add_buff_in({this->id, Buff::DEFAULT},
                                       Buff::attack(-0.5),
                                       clk.with_duration_sec(25));
                });
        }
    }
}

void AttackDown::on_death(GridRef) {
    // todo: remove buffs
}

} // namespace core
} // namespace towerdefence