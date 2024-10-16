#include "map.h"
#include "entity/entity.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace towerdefence {
namespace core {
GridRef Map::iterator::operator*() {
    auto [row, col] = m.shape.pos_of(std::distance(base, cur));
    return GridRef{m, *cur, row, col};
}

void Map::update() {
    clock_.on_tick();
    this->timeouts_.on_tick(this->clock(), *this);
    for (auto ref : *this) {
        auto &grid = ref.grid;
        if (grid.tower.has_value()) {
            auto &tower = grid.tower.value();
            tower->on_tick(ref);
        }
        for (auto &enemy : grid.enemies) {
            enemy->on_tick(ref);
        }
    }

    for (auto ref : *this) {
        auto pos = std::remove_if(
            ref.grid.enemies.begin(), ref.grid.enemies.end(),
            [](std::unique_ptr<Enemy> &enemy) {
                return enemy->realized_attack_ >= enemy->info().health_;
            });

        for (auto it = pos; it != ref.grid.enemies.end(); ++it) {
            (*it)->on_death(ref);
        }

        ref.grid.enemies.erase(pos, ref.grid.enemies.end());
    }
}

GridRef Map::get_ref(size_t row, size_t column) {
    return GridRef(*this, row, column);
}
} // namespace core
} // namespace towerdefence
