#include "weaken_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        WeakenMagicianPlus::WeakenMagicianPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}
        void WeakenMagicianPlus::on_tick(GridRef g) {
            Tower::on_tick(g);

            auto &clk = g.clock();

            auto status = this->status();
            // do normal attack
            if (clk.is_triggered(this->attack_)){
                auto grids = g.with_radius(status.attack_radius_, linf_dis);
                auto enemy_grid = get_enemy_grid(*this,grids);
                if(enemy_grid!=grids.end()){
                    single_attack(*this,*enemy_grid);
                }
            }

            // release skill
            if (clk.is_triggered(release_skill_)) {
                for (auto ref : g.with_radius(status.attack_radius_, linf_dis)) {
                    ref.grid.with_enemy([this, clk](Enemy &enemy) {
                        enemy.add_buff_in({this->id, Buff::DEFAULT},
                                          Buff::defence_correction({-1000,-40}),
                                          clk.with_duration_sec(20));
                    });
                }
                g.attack_enemies_in_radius(
                        status.with_attack(status.attack_ * 2.0), linf_dis);
            }
        }
    } // namespace core
} // namespace towerdefence