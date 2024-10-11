#include "bomber_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        BomberPlus::BomberPlus(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(20)) {}

        void BomberPlus::on_tick(GridRef g) {
            Tower::on_tick(g);
            size_t sputter_radius = 1;
            auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
            auto enemy_grid = get_enemy_grid(*this,grids);
            if(enemy_grid!=grids.end()){
                if(skill){
                    sputter_radius = 2;
                    this->add_buff({this->id, Buff::DEFAULT},
                                   Buff::attack(1.00));
                    for (auto ref : enemy_grid->with_radius(sputter_radius, l1_dis)) {
                        ref.grid.with_enemy([this](Enemy &enemy) {
                            enemy.increase_attack(this->status().attack_,this->status().attack_type_);
                        });
                    }
                    sputter_radius = 1;
                    this->remove_buff_from(this->id);
                    skill = false;
                }else{
                    for (auto ref : enemy_grid->with_radius(sputter_radius, l1_dis)) {
                        ref.grid.with_enemy([this](Enemy &enemy) {
                            enemy.increase_attack(this->status().attack_,this->status().attack_type_);
                        });
                    }
                }
            }

            if (g.clock().is_triggered(release_skill_)) {
                skill = true;
            }
        }
    } // namespace core
} // namespace towerdefence