#ifndef TOWERDEFENCE_CORE_MAP
#define TOWERDEFENCE_CORE_MAP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "./entity/entity.h"
#include "./id.h"
#include "timer.h"

namespace towerdefence {
namespace core {
struct Grid {
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::optional<std::unique_ptr<Tower>> tower;

    void with_tower(std::function<void(std::unique_ptr<Tower> &)> f) {
        if (tower.has_value()) {
            f(tower.value());
        }
    }

    void with_enemy(std::function<void(Enemy &)> f) {
        for (auto &enemy : enemies) {
            f(*enemy);
        }
    }

    template <class T>
    std::optional<T> with_enemy_id(id::Id id, std::function<T(Enemy &)> f) {
        if (auto it = std::find_if(enemies.begin(), enemies.end(),
                                   [id](auto &en) { return en->id == id; });
            it != enemies.end()) {
            return f(**it);
        }

        return {};
    }

    bool with_enemy_id(id::Id id, std::function<void(Enemy &)> f) {
        return with_enemy_id<std::tuple<>>(id,
                                           [f](auto &enemy) {
                                               f(enemy);
                                               return std::tuple{};
                                           })
            .has_value();
    }

    // Removes enemy whose id == `id`
    // Returns true if such enemy exists, otherwise, false s returned.
    bool remove_enemy(id::Id id) {
        if (auto it = std::find_if(enemies.cbegin(), enemies.cend(),
                                   [id](auto &en) { return en->id == id; });
            it != enemies.cend()) {
            enemies.erase(it);
            return true;
        }

        return false;
    }
};

struct Shape {
    size_t width_;
    size_t height_;

    explicit Shape(size_t width, size_t height)
        : width_(width), height_(height) {}

    size_t index_of(size_t row, size_t column) const {
        return row * width_ + column;
    }
};

struct GridRef;

struct Map {
  private:
    timer::Clock clock_;
    id::IdGenerator id_gen;
    // INVARIANCE: `id` in `enemy_refs_` <=> `grid` at `enemy_refs_[id]` has an
    // entity of `id`
    std::unordered_map<id::Id, std::pair<size_t, size_t>> enemy_refs_;
    std::unordered_map<id::Id, std::pair<size_t, size_t>> tower_refs_;

  public:
    std::vector<Grid> grids;
    Shape shape;

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

    id::Id spawn_enemy_at(size_t row, size_t column, EnemyFactoryBase &enemy) {
        auto &grid = grids.at(shape.index_of(row, column));
        auto id = assign_id();
        grid.enemies.push_back(enemy.construct(id, clock()));

        enemy_refs_.insert({id, {row, column}});

        return id;
    }

    // throws std::out_of_range if id does not exist
    void remove_enemy(id::Id id) {
        auto [row, column] = enemy_refs_.at(id);
        auto &grid = grids.at(shape.index_of(row, column));
        auto res = grid.remove_enemy(id);
        assert(res);
        enemy_refs_.erase(id);
    }

    // throws std::out_of_range if id does not exist
    Enemy &get_enemy_by_id(id::Id id) {
        auto [row, column] = enemy_refs_.at(id);
        auto &grid = grids.at(shape.index_of(row, column));
        return grid
            .with_enemy_id<std::reference_wrapper<Enemy>>(
                id, [](Enemy &enemy) { return std::reference_wrapper{enemy}; })
            .value();
    }

    // throws std::out_of_range if id does not exist
    Tower &get_tower_by_id(id::Id id) {
        auto [row, column] = tower_refs_.at(id);
        auto &grid = grids.at(shape.index_of(row, column));

        assert(grid.tower.value()->id == id);

        return **grid.tower;
    }

    id::Id assign_id() { return id_gen.gen(); }

    const timer::Clock &clock() const { return clock_; }

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
        : map(m), grid(m.grids[m.shape.index_of(row_, column_)]), row(row_),
          column(column_) {}

    // Returns points whose distance between self <= radix
    std::vector<GridRef>
    with_radius(size_t radius,
                std::function<size_t(size_t, size_t, size_t, size_t)> dis) {
        std::vector<GridRef> res;
        // todo: optimize to O(radix) algorithm
        for (size_t i = 0; i < map.shape.height_; ++i) {
            for (size_t j = 0; j < map.shape.width_; ++j) {
                if (dis(row, column, i, j) <= radius) {
                    res.emplace_back(map, i, j);
                }
            }
        }

        return res;
    }

    const timer::Clock &clock() const { return map.clock(); }
    Grid &current() { return grid; }
};

} // namespace core
} // namespace towerdefence

#endif