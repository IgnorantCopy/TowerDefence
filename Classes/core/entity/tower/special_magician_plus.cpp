#include "special_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        SpecialMagicianPlus::SpecialMagicianPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}
        void SpecialMagicianPlus::on_tick(GridRef g) {
            Tower::on_tick(g);
            auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
            auto enemy_grid = get_enemy_grid(*this,grids);
            if(enemy_grid!=grids.end()){
                single_attack(*this,*enemy_grid);
            }//todo
        }
    } // namespace core
} // namespace towerdefence