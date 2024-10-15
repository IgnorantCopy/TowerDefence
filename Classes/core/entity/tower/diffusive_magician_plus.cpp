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

            ref.attack_enemies_in_radius(this->status().with_attack_radius(1),
                                         linf_dis, g);
        }
    }

    if (clk.is_triggered(this->release_skill_)&&!get_all_buff().silent_) {
        this->attack_ = clk.never();
        this->add_buff_in({this->id, Buff::DEFAULT},
                          Buff::attack_radius(-1) & Buff::attack(2.2),
                          clk.with_duration_sec(20));

        g.on_tower_release_skill(*this, g.map, 20);

        timeouts_.add_callback(clk.with_before_sec(20),
                               [](DiffusiveMagicianPlus &self, GridRef g) {
                                   auto status = self.status();
                                   g.attack_enemies_in_radius(
                                       status.with_attack(status.attack_ * 1.2),
                                       linf_dis, g);

                                   return true;
                               });
        timeouts_.add_callback(clk.with_duration_sec(20),
                               restore_normal_attack);
    }
}
} // namespace core
} // namespace towerdefence