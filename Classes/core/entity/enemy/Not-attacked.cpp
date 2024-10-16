#include "Not-attacked.h"
#include "../../map.h"

namespace towerdefence {
namespace core {

NotAttacked::NotAttacked(id::Id id, route::Route route) : Enemy(id, route) {}

void NotAttacked::on_hit(int32_t atk, AttackType attack_type, GridRef g) {
    Enemy::on_hit(atk, attack_type, g);
    if (this->not_hit_) {
        this->add_buff_in({this->id, Buff::INVINCIBLE}, Buff::invincible(true),g.clock().with_duration_sec(5));
        this->not_hit_ = false;
        g.on_enemy_release_skill(*this, g.map, 5);
    }
}

} // namespace core
} // namespace towerdefence