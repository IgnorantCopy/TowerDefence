#include "helper_base.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        HelperBase::HelperBase(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(30)) {}

        void HelperBase::on_tick(GridRef g) {
            Tower::on_tick(g);

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack(0.20),
                                  g.clock().with_duration_sec(15));
            }
        }

    } // namespace core
} // namespace towerdefence