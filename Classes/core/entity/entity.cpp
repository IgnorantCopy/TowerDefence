#include "entity.h"
#include "../map.h"

namespace towerdefence::core {

void Entity::on_death(GridRef g) {}
void Entity::on_tick(GridRef g) {}
void Entity::on_hit(GridRef g) {}
void Entity::on_attack(GridRef g) {}

void Enemy::on_tick(GridRef g) {
    this->update_buff(g.clock());
}

void Tower::on_tick(GridRef g) {
    this->update_buff(g.clock());
}
} // namespace towerdefence::core