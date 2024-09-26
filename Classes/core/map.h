#ifndef TOWERDEFENCE_CORE_MAP
#define TOWERDEFENCE_CORE_MAP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "./entity/entity.h"
#include "./id.h"

namespace towerdefence {
namespace core {
struct Grid {
    std::vector<Enemy> enemies;
    std::optional<std::unique_ptr<Tower>> tower;

    void with_tower(std::function<void(std::unique_ptr<Tower> &)> f) {
        if (tower.has_value()) {
            f(tower.value());
        }
    }

    void with_enemy(std::function<void(Enemy &)> f) {
        for (auto &enemy: enemies) {
            f(enemy);
        }
    }
};

struct Shape {
    size_t width_;
    size_t height_;

    explicit Shape(size_t width, size_t height)
        : width_(width), height_(height) {}

    size_t get_index(size_t row, size_t column) const {
        return row * width_ + column;
    }
};

struct GridRef;

struct Map {
    std::vector<Grid> grids;
    Shape shape;

    id::IdGenerator id_gen;

    explicit Map(std::vector<Grid> &&grids_, size_t width, size_t height)
        : grids(std::move(grids_)), shape{width, height} {
        assert(width * height == grids_.size());
    }

    explicit Map(size_t width_, size_t height_,
                 std::function<Grid(size_t, size_t)> f)
        : shape{width_, height_} {
        grids.reserve(width_ * height_);
        for (size_t i = 0; i < height_; ++i) {
            for (size_t j = 0; j < width_; ++j) {
                grids.push_back(f(i, j));
            }
        }
    }

    GridRef get_ref(size_t row, size_t column);

    void update();
};

static size_t absdiff(size_t x, size_t y) { return (x > y) ? x - y : y - x; }

// ||x1 - x2|| + ||y1 - y2||
static size_t l1_dis(size_t x1, size_t y1, size_t x2, size_t y2) {
    return absdiff(x1, x2) + absdiff(y1, y2);
}

// max(||x1 - x2||, ||y1 - y2||)
static size_t linf_dis(size_t x1, size_t y1, size_t x2, size_t y2) {
    return std::max(absdiff(x1, x2), absdiff(y1, y2));
}

struct GridRef {
    Map &map;
    Grid &grid;
    size_t row;
    size_t column;

    // SAFETY: (row_, column_) must be a valid index
    explicit GridRef(Map &m, size_t row_, size_t column_)
        : map(m), grid(m.grids[m.shape.get_index(row_, column_)]), row(row_),
          column(column_) {}

    // Returns points whose distance between self <= radix
    std::vector<GridRef>
    with_radix(size_t radix,
               std::function<size_t(size_t, size_t, size_t, size_t)> dis) {
        std::vector<GridRef> res;
        // todo: optimize to O(radix) algorithm
        for (size_t i = 0; i < map.shape.height_; ++i) {
            for (size_t j = 0; j < map.shape.width_; ++j) {
                if (dis(row, column, i, j) <= radix) {
                    res.emplace_back(map, i, j);
                }
            }
        }

        return res;
    }
};

} // namespace core
} // namespace towerdefence

#endif