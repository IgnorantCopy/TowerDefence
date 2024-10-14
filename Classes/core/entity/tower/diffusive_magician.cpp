#include "diffusive_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        DiffusiveMagician::DiffusiveMagician(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}

        void DiffusiveMagician::on_tick(GridRef g) {
            Tower::on_tick(g);
            if (g.clock().is_triggered(attack_)){
                auto grids = g.with_radius(this->status().attack_radius_, linf_dis);

                if (auto it = grid_of_nearest_enemy(grids); it != grids.end()) {
                    auto ref = *it;

                    ref.attack_enemies_in_radius(this->status().with_attack_radius(1),
                                                 linf_dis);
                }
            }


            if (g.clock().is_triggered(release_skill_)&&!get_all_buff().silent_) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack_speed(25),
                                  g.clock().with_duration_sec(10));
            }
        }

    } // namespace core
} // namespace towerdefence