#include "worm.h"
#include "../../map.h"
#include "../entity.h"

#include <memory>

namespace towerdefence {
namespace core {

void Worm::on_death(GridRef g) {
    Enemy::on_death(g);
    for (auto grid : g.with_radius(1, linf_dis)) {
        grid.current().with_tower(
            [this, &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                tower->add_buff_in({this->id, Buff::DECREASE_SPEED},
                                   Buff::attack_speed(-50),
                                   clk.with_duration_sec(10));
            });
    }
}

} // namespace core
} // namespace towerdefence
