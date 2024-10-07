#include "highspeed_archor_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        HighspeedArcherPlus::HighspeedArcherPlus(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(5)) {}

        void HighspeedArcherPlus::on_tick(GridRef g) {
            this->update_buff(g.clock());

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff({this->id, Buff::DEFAULT},
                                  Buff::attack(0.6));
            }
        }

    } // namespace core
} // namespace towerdefence