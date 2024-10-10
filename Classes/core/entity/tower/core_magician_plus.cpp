#include "core_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        CoreMagicianPlus::CoreMagicianPlus(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(35)) {}

        void CoreMagicianPlus::on_tick(GridRef g) {
            Tower::on_tick(g);

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack_radius(1)&Buff::attack(2.30),
                                  g.clock().with_duration_sec(10));
            }
        }

    } // namespace core
} // namespace towerdefence