#include "archer_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        ArcherPlus::ArcherPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(45)) {}

        void ArcherPlus::on_tick(GridRef g) {
            Tower::on_tick(g);
            if(g.clock().is_triggered(attack_)){
                auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);

                if(enemy_grid!=grids.end()){
                    single_attack(*this,*enemy_grid);
                }
            }

            if (g.clock().is_triggered(release_skill_)&&!get_all_buff().silent_) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack(1.00)&Buff::real_attack(0.10),
                                  g.clock().with_duration_sec(20));
                g.on_tower_release_skill(*this, g.map, 20);
            }
        }

    } // namespace core
} // namespace towerdefence