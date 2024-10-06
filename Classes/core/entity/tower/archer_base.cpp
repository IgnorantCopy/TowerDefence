#include "archer_base.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        archorBase::archorBase(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(20)) {}

        void archorBase::on_tick(GridRef g) {
            this->update_buff(g.clock());

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack(0.25),
                                  g.clock().with_duration_sec(20));
            }
        }

    } // namespace core
} // namespace towerdefence