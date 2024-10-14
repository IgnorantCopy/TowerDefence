#include "bomber.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        Bomber::Bomber(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(8)) {}

        void Bomber::on_tick(GridRef g) {
            Tower::on_tick(g);
            size_t sputter_radius = 1;
            if(g.clock().is_triggered(attack_)){
                auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);
                if(enemy_grid!=grids.end()){
                    if(skill){
                        sputter_radius = 2;
                        for (auto ref : enemy_grid->with_radius(sputter_radius, l1_dis)) {
                            ref.grid.with_enemy([this](Enemy &enemy) {
                                enemy.increase_attack(this->status().attack_,this->status().attack_type_);
                            });
                        }
                        sputter_radius = 1;
                        skill = false;
                    }else{
                        for (auto ref : enemy_grid->with_radius(sputter_radius, l1_dis)) {
                            ref.grid.with_enemy([this](Enemy &enemy) {
                                enemy.increase_attack(this->status().attack_,this->status().attack_type_);
                            });
                        }
                    }
                }
            }

            if (g.clock().is_triggered(release_skill_)&&!get_all_buff().silent_) {
                skill = true;
            }
        }
    } // namespace core
} // namespace towerdefence