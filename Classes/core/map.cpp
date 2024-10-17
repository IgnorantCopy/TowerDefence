#include "map.h"
#include "entity/entity.h"
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

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
            assert(this->tower_refs_.count(tower->id) > 0);
            tower->on_tick(ref);
        }

        for (auto &enemy : grid.enemies) {
            assert(enemy && this->enemy_refs_.count(enemy->id) > 0);
            enemy->on_tick(ref);
        }
    }

    for (auto ref : *this) {

        std::erase_if(ref.grid.enemies, [ref, this](std::unique_ptr<Enemy>
                                                        &enemy) mutable {
            if (enemy->realized_attack_ >= enemy->status().health_) {
                enemy->on_death(ref);
                
                return true;
            }

            assert(enemy->move_progress_ >= 0 && enemy->move_progress_ <= 1);

            enemy->move_progress_ +=
                0.1 * enemy->status().speed_ / timer::TICK_PER_SECOND;

            assert(enemy->move_progress_ < 2);

            if (enemy->move_progress_ >= 1) {
                enemy->move_progress_ -= 1;

                try {
                    if (auto [dx, dy] = enemy->route_.next_direction();
                        dx != 0 && dy != 0) {
                        auto nx = route::ssize(ref.row) + dx;
                        auto ny = route::ssize(ref.column) + dy;
                        assert(nx >= 0 && nx < this->shape.height_ && ny >= 0 &&
                               ny < this->shape.width_);

                        // SAFETY: currently we only holds reference of
                        // `grid.enemies`, and (nx, ny) != (ref.row, ref.column)
                        auto &e =
                            this->relocate_enemy_at(std::move(enemy), nx, ny);
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
