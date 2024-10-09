#include "diffusive_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        DiffusiveMagician::DiffusiveMagician(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(20)) {}

        void DiffusiveMagician::on_tick(GridRef g) {
            Tower::on_tick(g);

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack_speed(25),
                                  g.clock().with_duration_sec(10));
            }
        }

    } // namespace core
} // namespace towerdefence