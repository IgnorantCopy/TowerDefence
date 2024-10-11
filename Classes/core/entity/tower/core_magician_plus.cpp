#include "core_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
namespace core {
CoreMagicianPlus::CoreMagicianPlus(id::Id id, const timer::Clock &clk)
    : Tower(id), release_skill_(clk.with_period_sec(35)),
      attack_(clk.with_period_sec(INFO.attack_interval_)) {}

void CoreMagicianPlus::on_tick(GridRef g) {
    Tower::on_tick(g);

    auto &clk = g.clock();

    if (clk.is_triggered(this->attack_)) {
        auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
        if (auto it = grid_of_nearest_enemy(grids); it != grids.end()) {
            auto grid = *it;
            grid.with_nearest_enemy([this, &clk](Enemy &e) {
                auto status = this->status();

                e.increase_attack(status.attack_, status.attack_type_);
                e.add_buff_in({this->id, Buff::DECREASE_SPEED},
                              Buff::attack_speed(-40),
                              clk.with_duration_sec(2));
            });
        }
    }

    if (clk.is_triggered(release_skill_)) {
        this->add_buff_in({this->id, Buff::DEFAULT},
                          Buff::attack_radius(1) & Buff::attack(2.30),
                          g.clock().with_duration_sec(10));
        auto status = this->status();
        for (auto ref : g.with_radius(status.attack_radius_, linf_dis)) {
            ref.grid.with_enemy([this, clk](Enemy &enemy) {
                enemy.add_buff_in({this->id, Buff::DECREASE_ATTACK_SPEED},
                                  Buff::attack_speed(-80),
                                  clk.with_duration_sec(35));
            });
        }

        this->attack_ = clk.never();
        this->timeouts_.add_callback(
            clk.with_duration_sec(35), [](CoreMagicianPlus &self, GridRef g) {
                self.attack_ =
                    g.clock().with_period(self.status().attack_interval_);
                return false;
            });
    }
}

} // namespace core
} // namespace towerdefence