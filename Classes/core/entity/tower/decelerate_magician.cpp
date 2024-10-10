#include "decelerate_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        DecelerateMagician::DecelerateMagician(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(30)) {}

        void DecelerateMagician::on_tick(GridRef g) {
            Tower::on_tick(g);

            if (g.clock().is_triggered(release_skill_)) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack(0.25)&Buff::attack_speed(25),
                                  g.clock().with_duration_sec(15));
            }
        }
    } // namespace core
} // namespace towerdefence