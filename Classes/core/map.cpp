#include "map.h"

namespace towerdefence {
namespace core {
void Map::update() {
    clock_.on_tick();
    for (size_t i = 0; i < shape.height_; ++i) {
        for (size_t j = 0; j < shape.width_; ++j) {
            auto ref = get_ref(i, j);
            auto &grid = ref.grid;
            if (grid.tower.has_value()) {
                auto &tower = grid.tower.value();
                tower->on_tick(ref);
            }
            for (auto &enemy : grid.enemies) {
                enemy.on_tick(ref);
            }
        }
    }
}

GridRef Map::get_ref(size_t row, size_t column) {
    return GridRef(*this, row, column);
}
} // namespace core
} // namespace towerdefence
