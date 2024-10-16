#include "archer_base.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        ArcherBase::ArcherBase(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(20)) {}

        void ArcherBase::on_tick(GridRef g) {
            Tower::on_tick(g);
            auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
            auto enemy_grid = get_enemy_grid(*this,grids);
            if(enemy_grid!=grids.end()){
                single_attack(*this,*enemy_grid);
            }

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack(0.25),
                                  g.clock().with_duration_sec(20));
            }
        }

    } // namespace core
} // namespace towerdefence