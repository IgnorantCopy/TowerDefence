#ifndef TOWERDEFENCE_CORE_MAP
#define TOWERDEFENCE_CORE_MAP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "./entity/entity.h"
#include "./id.h"
#include "timer.h"

namespace towerdefence {
    namespace core {
        struct Grid {
            enum Type {
                BlockPath,
                BlockIn,
                BlockOut,
                BlockTransport,
                BlockTower,
                None
            };
            Type type;
            std::vector<std::unique_ptr<Enemy>> enemies;
            std::optional<std::unique_ptr<Tower>> tower;
            
            explicit Grid() : type(None) {}
            
            explicit Grid(Type type) : type(type) {}
            
            void with_tower(std::function<void(std::unique_ptr<Tower> &)> f) {
                if (tower.has_value()) {
                    f(tower.value());
                }
            }
            
            void with_enemy(std::function<void(Enemy &)> f) {
                for (auto &enemy: enemies) {
                    f(*enemy);
                }
            }
            
            template<class T>
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
            std::optional<std::unique_ptr<Enemy>> remove_enemy(id::Id id) {
                if (auto it = std::find_if(enemies.begin(), enemies.end(),
                                           [id](auto &en) { return en->id == id; });
                        it != enemies.cend()) {
                    auto ret = std::unique_ptr<Enemy>{};
                    it->swap(ret);
                    enemies.erase(it);
                    return ret;
                }
                
                return {};
            }
            
            std::optional<std::unique_ptr<Tower>> remove_tower() {
                if (tower) {
                    std::optional<std::unique_ptr<Tower>> ret = std::nullopt;
                    tower.swap(ret);
                    return ret;
                } else {
                    return std::nullopt;
                }
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
            
            std::pair<size_t, size_t> pos_of(size_t idx) const {
                return {idx / width_, idx % width_};
            }
        };
        
        struct GridRef;
        
        struct CallbackHandle {
            id::Id handle;
            
            struct hasher {
                size_t operator()(const CallbackHandle &h) const noexcept {
                    return std::hash<id::Id>{}(h.handle);
                }
            };
            
            bool operator==(const CallbackHandle &) const noexcept = default;
        };
        
        template<class... Parma>
        using CallbackContainer =
                std::unordered_map<CallbackHandle, std::function<void(Parma...)>,
                        CallbackHandle::hasher>;
        
        struct Map {
        private:
            timer::Clock clock_;
            timer::Timer cost_timer_;
            id::IdGenerator id_gen;
            // INVARIANCE: `id` in `enemy_refs_` <=> `grid` at `enemy_refs_[id]` has an
            // entity of `id`
            std::unordered_map<id::Id, std::pair<size_t, size_t>> enemy_refs_;
            std::unordered_map<id::Id, std::pair<size_t, size_t>> tower_refs_;
            
            friend GridRef;
            
            struct {
                CallbackContainer<Enemy &, Tower &> on_enemy_attacked;
                CallbackContainer<Enemy &, towerdefence::core::Map &, uint32_t> on_enemy_release_skill;//uint32_t duration
                CallbackContainer<Tower &, towerdefence::core::Map &, uint32_t> on_tower_release_skill;
                CallbackContainer<Enemy &> on_enemy_death;
                CallbackContainer<Enemy &, std::pair<size_t, size_t>, std::pair<size_t, size_t>> on_enemy_move;
                CallbackContainer<id::Id> on_escape;
                CallbackContainer<bool> on_end;//bool is_win
            } callbacks_;
            
            timer::CallbackTimer<Map &> timeouts_;
        
        public:
            struct iterator {
                using base_iter = std::vector<Grid>::iterator;
                
                base_iter base;
                base_iter cur;
                Map &m;
                
                GridRef operator*();
                
                iterator operator++() {
                    ++cur;
                    return *this;
                }
                
                bool operator==(const iterator &rhs) const noexcept {
                    return cur == rhs.cur;
                }
                
                bool operator!=(const iterator &rhs) const noexcept {
                    return cur != rhs.cur;
                }
            };
            
            static constexpr uint32_t MAX_HEALTH = 10;
            
            std::vector<Grid> grids;
            Shape shape;
            
            uint32_t cost_ = 1000;
            uint32_t health_{MAX_HEALTH};
            uint32_t enemy_alive = 100;
            
            explicit Map(size_t width_, size_t height_,
                         std::function<Grid(size_t, size_t)> f)
                    : shape{width_, height_}, cost_timer_(clock_.with_period_sec(1)) {
                grids.reserve(width_ * height_);
                for (size_t i = 0; i < height_; ++i) {
                    for (size_t j = 0; j < width_; ++j) {
                        grids.push_back(f(i, j));
                    }
                }
            }
            
            // register a callback function to be called whenver an entity releases a
            // skill
            CallbackHandle
            on_enemy_release_skill(
                    std::function<void(Enemy &, towerdefence::core::Map &map, uint32_t duration)> f) {
                CallbackHandle handle{this->assign_id()};
                this->callbacks_.on_enemy_release_skill.insert({handle, f});
                return handle;
            }
            
            CallbackHandle
            on_tower_release_skill(
                    std::function<void(Tower &, towerdefence::core::Map &map, uint32_t duration)> f) {
                CallbackHandle handle{this->assign_id()};
                this->callbacks_.on_tower_release_skill.insert({handle, f});
                return handle;
            }
            
            // register a callback function to be called whenver an entity dies
            CallbackHandle
            on_enemy_death(std::function<void(Enemy &)> f) {
                CallbackHandle handle{this->assign_id()};
                this->callbacks_.on_enemy_death.insert({handle, f});
                return handle;
            }
            
            // register a callback function to be called whenver an entity moves
            CallbackHandle
            on_enemy_move(std::function<void(Enemy &, std::pair<size_t, size_t>, std::pair<size_t, size_t>)> f) {
                CallbackHandle handle{this->assign_id()};
                this->callbacks_.on_enemy_move.insert({handle, f});
                return handle;
            }
            
            CallbackHandle
            on_enemy_attacked(std::function<void(Enemy &, Tower &)> f) {
                CallbackHandle handle{this->assign_id()};
                this->callbacks_.on_enemy_attacked.insert({handle, f});
                return handle;
            }
            
            CallbackHandle on_escape(std::function<void(id::Id)> f) {
                CallbackHandle handle{this->assign_id()};
                this->callbacks_.on_escape.insert({handle, f});
                return handle;
            }

            CallbackHandle on_end(std::function<void(bool)> f) {
                CallbackHandle handle{this->assign_id()};
                this->callbacks_.on_end.insert({handle, f});
                return handle;
            }
            
            GridRef get_ref(size_t row, size_t column);
            
            id::Id spawn_enemy_at(size_t row, size_t column, EnemyFactoryBase &enemy) {
                auto &grid = grids.at(shape.index_of(row, column));
                auto id = assign_id();
                grid.enemies.push_back(enemy.construct(id, clock()));
                
                enemy_refs_.insert({id, {row, column}});
                
                return id;
            }
            
            // SAFETY: caller must ensure the id will not be used anymore.
            void unregister_enemy_id(id::Id id) {
                assert(enemy_refs_.count(id) > 0);
                enemy_refs_.erase(id);
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
            
            void move_enemy_to(id::Id id, size_t row, size_t col) {
                auto [r, c] = enemy_refs_.at(id);
                auto &grid = grids.at(shape.index_of(r, c));
                
                auto enemy = grid.remove_enemy(id).value();
                auto &new_grid = grids.at(shape.index_of(row, col));
                new_grid.enemies.push_back(std::move(enemy));
                enemy_refs_[id] = {row, col};
            }
            
            void reached_end(id::Id id) {
                for (auto &[handle, f]: this->callbacks_.on_escape) {
                    f(id);
                }
                this->health_ -= 1;
                if(this->health_ == 0){
                    for (auto &[handle, f]: this->callbacks_.on_end) {
                        f(false);
                    }
                }
                this->enemy_alive -= 1;
                if(this->enemy_alive == 0){
                    for (auto &[handle, f]: this->callbacks_.on_end) {
                        f(true);
                    }
                }
            }
            
            std::optional<id::Id> spawn_tower_at(size_t row, size_t column,
                                                 TowerFactoryBase &tower) {
                auto &grid = grids.at(shape.index_of(row, column));
                
                if (grid.tower.has_value()) {
                    return {};
                }
                
                auto info = tower.info();
                if (info.cost_ > this->cost_) {
                    return {};
                }
                
                this->cost_ -= info.cost_;
                
                auto id = assign_id();
                grid.tower = tower.construct(id, clock());
                
                tower_refs_.insert({id, {row, column}});
                
                return id;
            }
            
            // throws std::out_of_range if id does not exist
            Tower &get_tower_by_id(id::Id id) {
                auto [row, column] = tower_refs_.at(id);
                auto &grid = grids.at(shape.index_of(row, column));
                
                assert(grid.tower.value()->id == id);
                
                return **grid.tower;
            }
            
            std::unique_ptr<Tower> remove_tower(id::Id id) {
                auto [row, column] = tower_refs_.at(id);
                auto &grid = grids.at(shape.index_of(row, column));
                
                assert(grid.tower.value()->id == id);
                auto tower = grid.remove_tower();
                tower_refs_.erase(id);
                
                return std::move(tower.value());
            }
            
            void withdraw_tower(id::Id id) {
                auto tower = remove_tower(id);
                cost_ += tower->info().cost_ / 2;
            }
            
            id::Id assign_id() { return id_gen.gen(); }
            
            const timer::Clock &clock() const { return clock_; }
            
            void update();
            
            iterator begin() { return iterator{grids.begin(), grids.begin(), *this}; }
            
            iterator end() { return iterator{grids.begin(), grids.end(), *this}; }
            
            // add a callback called when t tiggers.
            // if callback returns false, it will be removed.
            //
            // SAFETY: caller must ensure that all captured variables of callback's
            // lifetime NOT SHORTER than the object.
            //
            // Particularly, do not capture members in `Tower`s or `Enemy`s.
            void set_timeout(timer::Timer t, std::function<bool(Map &)> callback) {
                this->timeouts_.add_callback(t, callback);
            }
            
            uint32_t getcost_() const { return cost_; }
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
        
        using DisFn = std::function<size_t(size_t, size_t, size_t, size_t)>;
        
        struct GridRef {
            Map &map;
            Grid &grid;
            size_t row;
            size_t column;
            
            // SAFETY: (row_, column_) must be a valid index
            explicit GridRef(Map &m, size_t row_, size_t column_)
                    : GridRef(m, m.grids[m.shape.index_of(row_, column_)], row_, column_) {}
            
            explicit GridRef(Map &m, Grid &g, size_t row_, size_t column_)
                    : map(m), grid(g), row(row_), column(column_) {}
            
            GridRef(const GridRef &) = default;
            
            // Returns points whose distance between self <= radix
            std::vector<GridRef> with_radius(size_t radius, DisFn dis) {
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
            
            // attack all enemies in grids found by (status.attack_radius_, dis)
            void attack_enemies_in_radius(TowerInfo status, DisFn dis, GridRef g) {
                for (auto ref: this->with_radius(status.attack_radius_, dis)) {
                    ref.grid.with_enemy([&](Enemy &e) {
                        e.on_hit(status.attack_, status.attack_type_, g);
                    });
                }
            }
            
            void with_nearest_enemy(std::function<void(Enemy &)> f) {
                auto &enemies = this->grid.enemies;
                auto target_enemy = std::ranges::min_element(
                        enemies.begin(), enemies.end(), {},
                        [](std::unique_ptr<Enemy> &e) { return e->remaining_distance(); });
                
                if (target_enemy != enemies.end()) {
                    f(**target_enemy);
                }
            }
            
            template<class... Args>
            requires std::is_invocable_v<typename decltype(map.callbacks_.on_enemy_move)::mapped_type, Args...>
            void on_enemy_move(Args &&... args) {
                for (auto &[id, f]: map.callbacks_.on_enemy_move) {
                    f(std::forward<Args>(args)...);
                }
            }
            
            template<class... Args>
            requires std::is_invocable_v<typename decltype(map.callbacks_.on_enemy_attacked)::mapped_type, Args...>
            void on_enemy_attacked(Args &&... args) const {
                for (auto &[id, f]: map.callbacks_.on_enemy_attacked) {
                    f(std::forward<Args>(args)...);
                }
            }
            
            template<class... Args>
            requires std::is_invocable_v<typename decltype(map.callbacks_.on_enemy_death)::mapped_type, Args...>
            void on_enemy_death(Args &&... args) {
                for (auto &[id, f]: map.callbacks_.on_enemy_death) {
                    f(std::forward<Args>(args)...);
                }
            }
            
            template<class... Args>
            requires std::is_invocable_v<typename decltype(map.callbacks_.on_enemy_release_skill)::mapped_type, Args...>
            void on_enemy_release_skill(Args &&... args) {
                for (auto &[id, f]: map.callbacks_.on_enemy_release_skill) {
                    f(std::forward<Args>(args)...);
                }
            }
            
            template<class... Args>
            requires std::is_invocable_v<typename decltype(map.callbacks_.on_tower_release_skill)::mapped_type, Args...>
            void on_tower_release_skill(Args &&... args) {
                for (auto &[id, f]: map.callbacks_.on_tower_release_skill) {
                    f(std::forward<Args>(args)...);
                }
            }
            
            template<class... Args>
            requires std::is_invocable_v<typename decltype(map.callbacks_.on_escape)::mapped_type, Args...>
            void on_escape(Args &&... args) {
                for (auto &[id, f]: map.callbacks_.on_escape) {
                    f(std::forward<Args>(args)...);
                }
            }

            template<class... Args>
                requires std::is_invocable_v<typename decltype(map.callbacks_.on_end)::mapped_type, Args...>
            void on_end(Args &&... args) {
                for (auto &[id, f]: map.callbacks_.on_end) {
                    f(std::forward<Args>(args)...);
                }
            }
            
            std::optional<std::unique_ptr<Tower>> &get_nearest_tower() {
                size_t r = 0, c = 0;
                for (auto [i, j]: std::views::cartesian_product(
                        std::views::iota(size_t(0), map.shape.height_),
                        std::views::iota(size_t(0), map.shape.width_))) {
                    if (l1_dis(row, column, i, j) < l1_dis(row, column, r, c) &&
                        map.grids[map.shape.index_of(i, j)].tower.has_value()) {
                        r = i;
                        c = j;
                    }
                }
                
                return map.grids[map.shape.index_of(r, c)].tower;
            }
            
            // if timer is triggered, call f(tower) for all towers on the map
            void for_each_tower_on_trigger(timer::Timer timer,
                                           std::function<void(Tower &)> f) {
                if (clock().is_triggered(timer)) {
                    for (auto &grid: map.grids) {
                        grid.with_tower([f](auto &t) { f(*t); });
                    }
                }
            }
            
            id::Id spawn_enemy(EnemyFactoryBase &enemy) {
                return this->map.spawn_enemy_at(this->row, this->column, enemy);
            }
            
            std::optional<id::Id> spawn_tower(TowerFactoryBase &tower) {
                return this->map.spawn_tower_at(this->row, this->column, tower);
            }
            
            const timer::Clock &clock() const { return map.clock(); }
            
            Grid &current() { return grid; }
            
            // add a callback called when t tiggers.
            // if callback returns false, it will be removed.
            //
            // SAFETY: caller must ensure that all captured variables of callback's
            // lifetime NOT SHORTER than the object.
            //
            // Particularly, do not capture members in `Tower`s or `Enemy`s.
            void set_timeout(timer::Timer t, std::function<bool(Map &)> callback) {
                this->map.set_timeout(t, callback);
            }
        };
        
    } // namespace core
} // namespace towerdefence

#endif