#include "Life-up.h"
#include "../../map.h"

namespace towerdefence {
namespace core {

void LifeUp::on_tick(GridRef g) {
    for (auto ref : g.with_radius(2, l1_dis)) {
        ref.grid.with_enemy([](Enemy &enemy) {
            auto health = enemy.status().health_;
            enemy.increase_attack(-health * 1.1,AttackType::Real);
        });
    }
}

} // namespace core
} // namespace towerdefence