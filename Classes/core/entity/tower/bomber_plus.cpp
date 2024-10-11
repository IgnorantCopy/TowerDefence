#include "bomber_plus.h"
#include "../../map.h"

namespace towerdefence {
namespace core {
BomberPlus::BomberPlus(id::Id id, const timer::Clock &clk)
    : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}

void BomberPlus::on_tick(GridRef g) {
    Tower::on_tick(g);
    auto status = this->status();
    // update interval
    this->attack_.visit_period([status](timer::Timer::Period &p) {
        p.period = status.attack_interval_;
    });
    this->timeouts_.on_tick(g.clock(), *this, g);

    auto &clk = g.clock();

    // do normal attack
    if (clk.is_triggered(this->attack_)) {
        auto grids = g.with_radius(status.attack_radius_, linf_dis);

        if (auto it = get_enemy_grid(*this, grids); it != grids.end()) {
            auto enemy_grid = *it;
            enemy_grid.attack_enemies_in_radius(status.with_attack_radius(1),
                                                linf_dis);
        }
    }

    // release skill
    if (clk.is_triggered(release_skill_)) {
        this->attack_ = clk.never(); // stop attacking
        // after 3s
        this->timeouts_.add_callback(
            clk.with_duration_sec(3), [](BomberPlus &self, GridRef g) {
                auto &clk = g.clock();
                auto status = self.status();

                // do 200% damage
                g.attack_enemies_in_radius(
                    status.with_attack(status.attack_ * 2), linf_dis);

                // increase attack speed in next 30s
                self.add_buff_in({self.id, Buff::DEFAULT},
                                 Buff::attack_speed(30),
                                 clk.with_duration_sec(5));

                // restore normal attack
                self.attack_ = clk.with_period_sec(status.attack_interval_);

                return false;
            });
    }
}
} // namespace core
} // namespace towerdefence