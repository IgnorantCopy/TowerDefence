#include "special_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        SpecialMagicianPlus::SpecialMagicianPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}
        void SpecialMagicianPlus::on_tick(GridRef g) {
            Tower::on_tick(g);
            this->timeouts_.on_tick(g.clock(), *this, g);
            auto &clk = g.clock();

            auto status = this->status();

            if (clk.is_triggered(this->attack_)){
                auto grids = g.with_radius(status.attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);
                if(enemy_grid!=grids.end()){
                    if(skill){
                        this->add_buff({this->id, Buff::DEFAULT},
                                       Buff::attack(1.50 * layer));
                        SpecialMagicianPlus &self = *this;
                        auto buffs = self.get_all_buff();
                        enemy_grid->with_nearest_enemy([&self, buffs, g](Enemy &target_enemy) {
                            target_enemy.add_buff({self.id, Buff::DEFAULT},
                                                  Buff::defence_correction({0,-50}));
                            target_enemy.on_hit(self.status().attack_,
                                                self.status().attack_type_,g);
                            target_enemy.remove_buff_from(self.id);
                            if (buffs.real_attack_ > 0) {
                                target_enemy.on_hit(
                                        self.status().attack_ * buffs.real_attack_, AttackType::Real, g);
                            }
                        });
                        layer = 0;
                        this->remove_buff_from(this->id);
                        skill = false;
                    }else{
                        this->add_buff({this->id, Buff::DEFAULT},
                                       Buff::attack(1.00 * layer));
                        single_attack(*this,*enemy_grid);
                        layer = 0;
                        this->remove_buff_from(this->id);
                    }
                }else if(layer < 3){
                    ++layer;
                }
            }

            if (clk.is_triggered(this->release_skill_)&&!get_all_buff().silent_) {
                this->skill = true;
                g.on_tower_release_skill(*this, g.map, 15);
                timeouts_.add_callback(clk.with_duration_sec(15),
                                       [](SpecialMagicianPlus &self, GridRef){
                                                    self.skill = false;
                                                    return false;

                                       });
            }
        }
    } // namespace core
} // namespace towerdefence