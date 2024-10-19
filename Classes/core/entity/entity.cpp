#include "entity.h"
#include "../map.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <format>
#include <iostream>

namespace towerdefence::core {

void Entity::on_death(GridRef g) {}
void Entity::on_tick(GridRef g) {}

void Enemy::on_hit(int32_t atk, AttackType attack_type, GridRef g) {
    switch (attack_type) {
    case AttackType::Physics:
        atk = atk>status().defence_.physics_?(atk-status().defence_.physics_):(atk/20);
        break;
    case AttackType::Magic:
        atk = atk * ((100-status().defence_.magic_)/100.0);
        break;
    default:
        break;
    }
    if(atk > 0){
        if(get_all_buff().invincible_){
            return;
        }
    }
    realized_attack_ += atk;
}

void Enemy::on_tick(GridRef g) {
    std::cout << std::format("{} (enemy): on tick", this->id.v);
    auto &clk = g.clock();

    this->update_buff(clk);
    auto speed = this->status().speed_;
    if (speed == 0) {
        this->move_.visit_period(
            [](timer::Timer::Period &p) { p.period = UINT32_MAX; });
    } else {
        this->move_.visit_period([interval = timer::TICK_PER_SECOND * 10. /
                                             this->status().speed_,
                                  &clk](timer::Timer::Period &p) {
            if (interval != p.period) {
                auto progress =
                    static_cast<double>((clk.elapased_ - p.start) % p.period) /
                    p.period;
                auto new_progress = static_cast<uint32_t>(progress * interval);
                p.start =
                    (p.start >= new_progress) ? (p.start - new_progress) : 0;
                p.period = interval;
            }
        });
    }
}

void Enemy::on_death(GridRef g) {
    g.on_enemy_death(*this);
    g.map.enemy_alive -= 1;
    if (g.map.enemy_alive == 0) {
        g.on_end(true);
    }
}

void Tower::on_tick(GridRef g) {
    std::cout << std::format("{} (tower): on tick", this->id.v);

    auto &clk = g.clock();
    this->update_buff(clk);
    this->attack_.visit_period(
        [interval = std::round(this->status().attack_interval_),
         &clk](timer::Timer::Period &p) {
            auto progress =
                static_cast<double>((clk.elapased_ - p.start) % p.period) /
                p.period;
            auto new_progress = static_cast<uint32_t>(progress * interval);
            std::cout << std::format("old progress: {}, new progress: {}", progress,
                          new_progress);
            std::cout << std::format("old p.start: {}, p.period: {}", p.start, p.period);

            p.start = (p.start >= new_progress) ? (p.start - new_progress) : 0;
            uint32_t next_period = interval;
            p.period = (next_period > 0) ? next_period : 1;
            std::cout << std::format("new p.start: {}, p.period: {}", p.start, p.period);
        });
}

std::vector<GridRef>::iterator get_enemy_grid(Tower &tower,
                                              std::vector<GridRef> &grids) {
    return grid_of_nearest_enemy(grids);
}

bool restore_normal_attack(Tower &self, GridRef g) {
    self.reset_attack_timer(g.clock());

    return false;
}

auto grid_of_nearest_enemy(std::vector<GridRef> &grids)
    -> std::vector<GridRef>::iterator {
    return std::min_element(
        grids.begin(), grids.end(), [](const GridRef &a, const GridRef &b) {
            auto a1 = std::min_element(
                a.grid.enemies.begin(), a.grid.enemies.end(),
                [](std::unique_ptr<Enemy> &x, std::unique_ptr<Enemy> &y) {
                    return x->remaining_distance() < y->remaining_distance();
                });
            auto b1 = std::min_element(
                b.grid.enemies.begin(), b.grid.enemies.end(),
                [](std::unique_ptr<Enemy> &x, std::unique_ptr<Enemy> &y) {
                    return x->remaining_distance() < y->remaining_distance();
                });
            if(a1 == a.grid.enemies.end()){
                return false;
            }else if(b1 == b.grid.enemies.end()){
                return true;
            }
            return (*a1)->remaining_distance() < (*b1)->remaining_distance();
        });
}

void single_attack(Tower &tower, GridRef enemy_grid) {
    auto buffs = tower.get_all_buff();
    if (buffs.attack_stop_) {
        return;
    }
    enemy_grid.with_nearest_enemy([&tower, buffs, enemy_grid](Enemy &target_enemy) mutable {
        target_enemy.on_hit(tower.status().attack_,
                            tower.status().attack_type_,enemy_grid);
        enemy_grid.on_enemy_attacked(target_enemy, tower);
        if (buffs.real_attack_ > 0) {
            target_enemy.on_hit(
                    tower.status().attack_ * buffs.real_attack_, AttackType::Real, enemy_grid);
        }
    });
}

} // namespace towerdefence::core