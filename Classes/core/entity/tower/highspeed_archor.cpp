#include "highspeed_archor.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        highspeedArcher::highspeedArcher(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(6)) {}

        void highspeedArcher::on_tick(GridRef g) {
            this->update_buff(g.clock());

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff({this->id, Buff::DEFAULT},
                                  Buff::inspiration_strike(0.4));
            }
        }

    } // namespace core
} // namespace towerdefence