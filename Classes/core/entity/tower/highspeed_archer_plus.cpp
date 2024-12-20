#include "highspeed_archer_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        HighspeedArcherPlus::HighspeedArcherPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(5)) {}

        void HighspeedArcherPlus::on_tick(GridRef g) {
            Tower::on_tick(g);
            if (g.clock().is_triggered(this->attack_)){
                auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);
                if(enemy_grid!=grids.end()){
                    if(skill){
                        this->add_buff({this->id, Buff::DEFAULT},
                                       Buff::attack(0.60));
                        single_attack(*this,*enemy_grid);
                        this->remove_buff_from(this->id);
                        skill = false;
                        g.on_tower_release_skill(*this, g.map, 0);
                    }else{
                        single_attack(*this,*enemy_grid);
                    }
                }
            }

            if (g.clock().is_triggered(release_skill_)&&!get_all_buff().silent_) {
                skill = true;
                g.on_tower_release_skill(*this, g.map, 1);
            }
        }
    } // namespace core
} // namespace towerdefence