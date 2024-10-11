#include "entity.h"
#include "../map.h"

namespace towerdefence::core {

void Entity::on_death(GridRef g) {}
void Entity::on_tick(GridRef g) {}
void Entity::on_hit(GridRef g) {}

void Enemy::increase_attack(int32_t atk, AttackType attack_type) {
    switch (attack_type) {
    case AttackType::Physics:
        atk *= status().defence_.physics_;
        break;
    case AttackType::Magic:
        atk *= status().defence_.magic_;
        break;
    default:
        break;
    }
    realized_attack_ += atk;
}

void Enemy::on_tick(GridRef g) { this->update_buff(g.clock()); }

void Tower::on_tick(GridRef g) { this->update_buff(g.clock()); }

std::vector<GridRef>::iterator get_enemy_grid(Tower &tower,
                                              std::vector<GridRef> &grids) {
    return grid_of_nearest_enemy(grids);
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
            return (*a1)->remaining_distance() < (*b1)->remaining_distance();
        });
}

void single_attack(Tower &tower, GridRef enemy_grid) {
    auto buffs = tower.get_all_buff();
    if (buffs.attack_stop_) {
        return;
    }
    auto target_enemy = std::min_element(
        enemy_grid.grid.enemies.begin(), enemy_grid.grid.enemies.end(),
        [](std::unique_ptr<Enemy> &x, std::unique_ptr<Enemy> &y) {
            return x->remaining_distance() < y->remaining_distance();
        });
    (*target_enemy)
        ->increase_attack(tower.status().attack_, tower.status().attack_type_);
    if (buffs.real_attack_ > 0) {
        (*target_enemy)
            ->increase_attack(tower.status().attack_ * buffs.real_attack_,
                              AttackType::Real);
    }
}

} // namespace towerdefence::core