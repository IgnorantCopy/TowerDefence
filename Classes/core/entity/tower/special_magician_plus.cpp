#include "special_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        SpecialMagicianPlus::SpecialMagicianPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}
        void SpecialMagicianPlus::on_tick(GridRef g) {
            Tower::on_tick(g);
            auto &clk = g.clock();

            auto status = this->status();
            // do normal attack
            if (clk.is_triggered(this->attack_)){
                auto grids = g.with_radius(status.attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);
                if(enemy_grid!=grids.end()){
                    this->add_buff({this->id, Buff::DEFAULT},
                                       Buff::attack(1.00 * layer));
                    single_attack(*this,*enemy_grid);
                    layer = 0;
                    this->remove_buff_from(this->id);
                }else if(layer < 3){
                    ++layer;
                }
            }

            if (clk.is_triggered(this->release_skill_)) {
                timeouts_.add_callback(clk.with_before_sec(15),
                                       [=](SpecialMagicianPlus &self, GridRef g) {
                                           if (clk.is_triggered(this->attack_)){
                                               auto grids = g.with_radius(status.attack_radius_, linf_dis);
                                               auto enemy_grid = get_enemy_grid(*this,grids);
                                               if(enemy_grid!=grids.end()){
                                                   this->add_buff({this->id, Buff::DEFAULT},
                                                                  Buff::attack(1.50 * layer));
                                                   auto buffs = this->get_all_buff();
                                                   enemy_grid->with_nearest_enemy([this, buffs](Enemy &target_enemy) {
                                                       target_enemy.add_buff({this->id, Buff::DEFAULT},
                                                                             Buff::defence_correction({0,-50}));
                                                       target_enemy.increase_attack(this->status().attack_,
                                                                                    this->status().attack_type_);
                                                       target_enemy.remove_buff_from(this->id);
                                                       if (buffs.real_attack_ > 0) {
                                                           target_enemy.increase_attack(
                                                                   this->status().attack_ * buffs.real_attack_, AttackType::Real);
                                                       }
                                                   });
                                                   layer = 0;
                                                   this->remove_buff_from(this->id);
                                               }else if(layer < 3){
                                                   ++layer;
                                               }
                                           }
                                           return true;
                                       });
            }
        }
    } // namespace core
} // namespace towerdefence