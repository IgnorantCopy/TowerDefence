#include "core_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        CoreMagician::CoreMagician(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(8)) {}

        void CoreMagician::on_tick(GridRef g) {
            this->update_buff(g.clock());

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff({this->id, Buff::DEFAULT},
                                  Buff::inspiration_strike(0.60));
            }
        }

    } // namespace core
} // namespace towerdefence