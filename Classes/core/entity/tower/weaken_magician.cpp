#include "weaken_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        WeakenMagician::WeakenMagician(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}
        void WeakenMagician::on_tick(GridRef g) {
            Tower::on_tick(g);

            auto &clk = g.clock();

            auto status = this->status();
            // do normal attack
            if (clk.is_triggered(this->attack_)) {
                auto grids = g.with_radius(status.attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);
                if(enemy_grid!=grids.end()){
                    single_attack(*this,*enemy_grid);
                }
            }

            // release skill
            if (clk.is_triggered(release_skill_)&&!get_all_buff().silent_) {
                for (auto ref : g.with_radius(status.attack_radius_, linf_dis)) {
                    ref.grid.with_enemy([this, clk](Enemy &enemy) {
                        enemy.add_buff_in({this->id, Buff::DEFAULT},
                                          Buff::defence_correction({-200,-10}),
                                          clk.with_duration_sec(10));
                    });
                }
                g.attack_enemies_in_radius(
                        status.with_attack(status.attack_ * 1.50), linf_dis);
            }
        }
    } // namespace core
} // namespace towerdefence