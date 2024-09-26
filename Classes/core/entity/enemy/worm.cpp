#include "worm.h"
#include "../../map.h"
#include "../entity.h"

#include <memory>

namespace towerdefence {
namespace core {

void Worm::on_tick(GridRef g) {}

void Worm::on_death(GridRef g) {
    for (auto grid : g.with_radix(1, linf_dis)) {
        grid.grid.with_tower([this](std::unique_ptr<Tower> &tower) {
            tower->add_buff(this->id, Buff::attack_speed(-50));
        });
    }
}

} // namespace core
} // namespace towerdefence
