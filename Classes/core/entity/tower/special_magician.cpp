#include "special_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        SpecialMagician::SpecialMagician(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(10)) {}
        void SpecialMagician::on_tick(GridRef g) {
            Tower::on_tick(g);
            auto &clk = g.clock();

            auto status = this->status();
            // do normal attack
            if (clk.is_triggered(this->attack_)){
                auto grids = g.with_radius(status.attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);
                if(enemy_grid!=grids.end()){
                    this->add_buff({this->id, Buff::DEFAULT},
                                   Buff::attack(0.50 * layer));
                    layer = 0;
                    if(skill){
                        enemy_grid->attack_enemies_in_radius(status.with_attack_radius(1).with_attack(status.attack_ * 1.5),
                                                                linf_dis);
                    }else{
                        single_attack(*this,*enemy_grid);
                    }
                    this->remove_buff_from(this->id);
                }else if(layer < 3){
                    ++layer;
                }
            }

            if (g.clock().is_triggered(release_skill_)&&!get_all_buff().silent_) {
                skill = true;
            }
        }
    } // namespace core
} // namespace towerdefence