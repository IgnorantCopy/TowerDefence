#include "entity.h"
#include "../map.h"
#include "route.h"
#include <cassert>
#include <stdexcept>

namespace towerdefence::core {

void Entity::on_death(GridRef g) {}
void Entity::on_tick(GridRef g) {}
void Entity::on_hit(GridRef g) {}

void Enemy::increase_attack(int32_t atk, AttackType attack_type) {
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
    realized_attack_ += atk;
}

void Enemy::on_tick(GridRef g) {
    auto &clk = g.clock();

    this->update_buff(clk);

    if (clk.is_triggered(this->move_)) {
        try {
            auto [dx, dy] = this->route_.next_direction();
            auto nx = route::ssize(g.row) + dx;
            auto ny = route::ssize(g.column) + dy;
            assert(nx >= 0 && nx < g.map.shape.height_ && ny >= 0 && ny < g.map.shape.width_);
            g.map.move_enemy_to(this->id, nx, ny);
            g.on_enemy_move(*this, std::make_pair(route::ssize(g.row), route::ssize(g.column)), std::make_pair(nx, ny));
        } catch (const std::out_of_range &) {
            g.map.reached_end(this->id);
        }
    }

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
    enemy_grid.with_nearest_enemy([&tower, buffs](Enemy &target_enemy) {
        target_enemy.increase_attack(tower.status().attack_,
                                     tower.status().attack_type_);
        if (buffs.real_attack_ > 0) {
            target_enemy.increase_attack(
                tower.status().attack_ * buffs.real_attack_, AttackType::Real);
        }
    });
}

} // namespace towerdefence::core