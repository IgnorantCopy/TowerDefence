#include "diffusive_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
namespace core {
void DiffusiveMagicianPlus::on_tick(GridRef g) {
    Tower::on_tick(g);

    auto &clk = g.clock();

    if (clk.is_triggered(this->attack_)) {
        auto grids = g.with_radius(this->status().attack_radius_, linf_dis);

        if (auto it = grid_of_nearest_enemy(grids); it != grids.end()) {
            auto ref = *it;

            this->attack_ = clk.never();
            this->add_buff_in({this->id, Buff::DEFAULT},
                              Buff::attack_radius(-1),
                              clk.with_duration_sec(20));

            timeouts_.add_callback(
                clk.with_before_sec(20),
                [](DiffusiveMagicianPlus &self, GridRef g) {
                    auto status = self.status();
                    g.attack_enemies_in_radius(
                        status.with_attack(status.attack_ * 1.2), linf_dis);

                    return true;
                });
            timeouts_.add_callback(clk.with_duration_sec(20),
                                   [](DiffusiveMagicianPlus &self, GridRef g) {
                                       auto status = self.status();
                                       self.attack_ = g.clock().with_period(
                                           status.attack_interval_);

                                       return false;
                                   });
        }
    }
}
} // namespace core
} // namespace towerdefence