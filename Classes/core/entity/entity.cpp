#include "entity.h"
#include "../map.h"
#include "route.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <stdexcept>

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
    auto &clk = g.clock();

    this->update_buff(clk);

    assert(this->move_progress_ >= 0 && this->move_progress_ <= 1);

    this->move_progress_ +=
        0.1 * this->status().speed_ / timer::TICK_PER_SECOND;

    auto moves = static_cast<uint32_t>(std::floor(this->move_progress_));
    this->move_progress_ = std::fmod(this->move_progress_, 1);

    for (uint32_t i = 0; i < moves; ++i) {
        try {
            auto [dx, dy] = this->route_.next_direction();
            auto nx = route::ssize(g.row) + dx;
            auto ny = route::ssize(g.column) + dy;
            assert(nx >= 0 && nx < g.map.shape.height_ && ny >= 0 &&
                   ny < g.map.shape.width_);
            g.map.move_enemy_to(this->id, nx, ny);
        } catch (const route::reached_end &) {
            g.map.reached_end(this->id); // todo: fix UAF in reached_end
            break;
        }
    }

    assert(this->move_progress_ >= 0 && this->move_progress_ <= 1);
}

void Enemy::on_death(GridRef g) {
    g.on_enemy_death(*this);
}

void Tower::on_tick(GridRef g) {
    this->update_buff(g.clock());
    this->attack_.visit_period(
        [status = this->status()](timer::Timer::Period &p) {
            p.period = status.attack_interval_;
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