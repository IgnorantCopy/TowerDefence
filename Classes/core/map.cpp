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
    if (clock_.is_triggered(cost_timer_)) {
        cost_++;
    }
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
        auto &enemies = ref.grid.enemies;

        auto pos = std::remove_if(
            enemies.begin(), enemies.end(), [](std::unique_ptr<Enemy> &enemy) {
                return enemy->realized_attack_ >= enemy->info().health_ ||
                       enemy->remaining_distance() == 0;
            });

        for (auto it = pos; it != enemies.end(); ++it) {
            auto &enemy = *it;
            enemy->on_death(ref);
            // unregister id from map
            this->unregister_enemy_id(enemy->id);
        }

        // erase enemies before, maintaining invariance of map
        enemies.erase(pos, enemies.end());
    }
}

GridRef Map::get_ref(size_t row, size_t column) {
    return GridRef(*this, row, column);
}
} // namespace core
} // namespace towerdefence
