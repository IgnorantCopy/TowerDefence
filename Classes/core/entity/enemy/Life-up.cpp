#include "Life-up.h"
#include "../../map.h"

namespace towerdefence {
namespace core {

void LifeUp::on_tick(GridRef g) {
    if (g.clock().is_triggered(release_skill_)) {
        for (auto ref : g.with_radius(2, l1_dis)) {
            ref.grid.with_enemy([](Enemy &enemy) {
                auto health = enemy.info().health_;
                enemy.increase_attack(-health * 0.1,AttackType::Real);
            });
        }
        g.on_enemy_release_skill(*this, g.map, 0);
    }
}

} // namespace core
} // namespace towerdefence