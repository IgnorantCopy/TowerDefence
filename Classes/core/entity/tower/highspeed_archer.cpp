#include "highspeed_archer.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        HighspeedArcher::HighspeedArcher(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(6)) {}

        void HighspeedArcher::on_tick(GridRef g) {
            Tower::on_tick(g);
            auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
            auto enemy_grid = get_enemy_grid(*this,grids);
            if(enemy_grid!=grids.end()){
                if(skill){
                    this->add_buff({this->id, Buff::DEFAULT},
                                   Buff::attack(0.40));
                    single_attack(*this,*enemy_grid);
                    this->remove_buff_from(this->id);
                    skill = false;
                }else{
                    single_attack(*this,*enemy_grid);
                }
            }

            if (g.clock().is_triggered(release_skill_)) {
                skill = true;
            }
        }
    } // namespace core
} // namespace towerdefence