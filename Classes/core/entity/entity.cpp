#include "entity.h"
#include "../map.h"

namespace towerdefence::core {

void Entity::on_death(GridRef g) {}
void Entity::on_tick(GridRef g) {}
void Entity::on_hit(GridRef g) {}

void Enemy::increase_attack(int32_t atk,AttackType attack_type){
    switch(attack_type){
        case AttackType::Physics:
            atk*=status().defence_.physics_;
            break;
        case AttackType::Magic:
            atk*=status().defence_.magic_;
            break;
        default:
            break;
    }
    realized_attack_ += atk;
}

void Enemy::on_tick(GridRef g) {
    this->update_buff(g.clock());
}

void Tower::on_tick(GridRef g) {
    this->update_buff(g.clock());
    auto grids = g.with_radius(status().attack_radius_, linf_dis);
    auto enemy_grid = std::min_element(grids.begin(),grids.end(),[](const GridRef &a,const GridRef &b){
        auto a1 = std::min_element(a.grid.enemies.begin(),a.grid.enemies.end(),[](std::unique_ptr<Enemy>& x, std::unique_ptr<Enemy>& y){
            return x->get_distance() < y->get_distance();
        });
        auto a2 = std::min_element(b.grid.enemies.begin(),b.grid.enemies.end(),[](std::unique_ptr<Enemy>& x, std::unique_ptr<Enemy>& y){
            return x->get_distance() < y->get_distance();
        });
        return (*a1)->get_distance()<(*a2)->get_distance();
    });
    if(enemy!=grids.end()){
        enemy
    }
}


} // namespace towerdefence::core