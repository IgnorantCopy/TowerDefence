#include "map.h"
#include "entity/entity.h"
#include "id.h"

#include <cassert>
#include <cstddef>
#include <format>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <vector>

namespace towerdefence {
namespace core {
GridRef Map::iterator::operator*() {
    auto [row, col] = m.shape.pos_of(std::distance(base, cur));
    return GridRef{m, *cur, row, col};
}

void Map::update() {
    clock_.on_tick();

    std::cout << std::format("tick {}:", this->clock_.elapased_) << std::endl;

    if (clock_.is_triggered(cost_timer_)) {
        cost_++;
    }
    this->timeouts_.on_tick(this->clock(), *this);

    std::cout << std::format("total enemies: {}", enemy_refs_.size()) << std::endl;

    for (auto ref : *this) {
        auto &grid = ref.grid;
        if (grid.tower.has_value()) {
            auto &tower = grid.tower.value();
            assert(this->tower_refs_.count(tower->id) > 0);
            tower->on_tick(ref);
        }

        for (auto &enemy : grid.enemies) {
            assert(enemy && this->enemy_refs_.count(enemy->id) > 0);
            enemy->on_tick(ref);
        }
    }

    std::unordered_set<id::Id> visited_enemies;

    for (auto ref : *this) {

        std::erase_if(
            ref.grid.enemies,
            [ref, &visited_enemies, this](std::unique_ptr<Enemy> &enemy) mutable {
                if (visited_enemies.contains(enemy->id)) {
                    return false;
                }

                visited_enemies.insert(enemy->id);

                if (enemy->realized_attack_ >= enemy->status().health_) {
                    enemy->on_death(ref);

                    return true;
                }

                // assert(enemy->move_progress_ >= 0 &&
                //        enemy->move_progress_ <= 1);

                // enemy->move_progress_ +=
                //     0.1 * enemy->status().speed_ / timer::TICK_PER_SECOND;

                // assert(enemy->move_progress_ < 2);

                if (this->clock().is_triggered(enemy->move_)) {

                    try {
                        if (auto [dx, dy] = enemy->route_.next_direction();
                            dx != 0 || dy != 0) {
                            std::cout << std::format("{}: moving", enemy->id.v) << std::endl;

                            auto nx = route::ssize(ref.row) + dx;
                            auto ny = route::ssize(ref.column) + dy;
                            assert(nx >= 0 && nx < this->shape.height_ &&
                                   ny >= 0 && ny < this->shape.width_);

                            // SAFETY: currently we only holds reference of
                            // `grid.enemies`, and (nx, ny) != (ref.row,
                            // ref.column)
                            auto &e = this->relocate_enemy_at(std::move(enemy),
                                                              nx, ny);
                            ref.on_enemy_move(
                                e, std::make_pair(ref.row, ref.column),
                                std::make_pair<size_t, size_t>(nx, ny));
                            return true;
                        }
                    } catch (const route::reached_end &) {
                        this->reached_end(enemy->id);

                        return true;
                    }
                }

                return false;
            });
    }
}

GridRef Map::get_ref(size_t row, size_t column) {
    return GridRef(*this, row, column);
}
} // namespace core
} // namespace towerdefence
