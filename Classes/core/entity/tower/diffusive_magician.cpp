#include "diffusive_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        diffusiveMagician::diffusiveMagician(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(20)) {}

        void diffusiveMagician::on_tick(GridRef g) {
            this->update_buff(g.clock());

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack_speed(25),
                                  g.clock().with_duration_sec(10));
            }
        }

    } // namespace core
} // namespace towerdefence