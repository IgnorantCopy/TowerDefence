#ifndef TOWERDEFENCE_CORE_MAP
#define TOWERDEFENCE_CORE_MAP

#include "./entity/entity.h"

#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace towerdefence {
namespace core {
struct Grid {
    std::vector<Enemy> enemies;
    std::unique_ptr<Tower> tower;
};

struct Shape {
    size_t width;
    size_t height;

    size_t get_index(size_t row, size_t column) const {
        return row * width + column;
    }
};

struct GridRef;

struct Map {
    std::vector<Grid> grids;
    Shape shape;

    explicit Map(std::vector<Grid> &&grids_, size_t width_, size_t height_)
        : grids(std::move(grids_)), shape{.width = width_, .height = height_} {
        assert(width_ * height_ == grids_.size());
    }

    explicit Map(size_t width_, size_t height_,
                 std::function<Grid(size_t, size_t)> f)
        : shape{.width = width_, .height = height_} {
        grids.reserve(width_ * height_);
        for (size_t i = 0; i < height_; ++i) {
            for (size_t j = 0; j < width_; ++j) {
                grids.push_back(f(i, j));
            }
        }
    }

    GridRef get_ref(size_t row, size_t column);
};

struct GridRef {
    Map &map;
    Grid &grid;
    size_t row;
    size_t column;

    // SAFETY: (row_, column_) must be a valid index
    explicit GridRef(Map &m, size_t row_, size_t column_)
        : map(m), grid(m.grids[m.shape.get_index(row_, column_)]), row(row_),
          column(column_) {}
};

} // namespace core
} // namespace towerdefence

#endif