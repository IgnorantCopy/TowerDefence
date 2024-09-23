#include "worm.h"
#include "../map.h"
#include "entity.h"

#include <memory>

namespace towerdefence {
namespace core {

void Worm::update(GridRef g) {
    for (auto grid : g.with_radix(1, linf_dis)) {
        grid.grid.with_tower([](std::unique_ptr<Tower> &tower) {
        });
    }
}

} // namespace core
} // namespace towerdefence
