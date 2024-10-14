#include "decelerate_magician.h"
#include "../../map.h"
#include <cstddef>

namespace towerdefence {
namespace core {
DecelerateMagician::DecelerateMagician(id::Id id, const timer::Clock &clk)
    : Tower(id, clk), release_skill_(clk.with_period_sec(30)) {}

void DecelerateMagician::on_tick(GridRef g) {
    Tower::on_tick(g);

    auto &clk = g.clock();

    if (clk.is_triggered(this->attack_)) {
        auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
        if (auto it = grid_of_nearest_enemy(grids); it != grids.end()) {
            auto grid = *it;
            grid.with_nearest_enemy([this, &clk](Enemy &e) {
                auto status = this->status();

                for (size_t i = 0; i < ((this->skill_) ? size_t(3) : size_t(1)); ++i) {
                    e.increase_attack(status.attack_, status.attack_type_);
                    e.add_buff_in({this->id, Buff::DECREASE_SPEED},
                                  Buff::speed(-40), clk.with_duration_sec(2));
                }
            });
        }
    }

    if (g.clock().is_triggered(release_skill_)&&!get_all_buff().silent_) {
        this->add_buff_in({this->id, Buff::DEFAULT},
                          Buff::attack(0.25) & Buff::attack_speed(25),
                          g.clock().with_duration_sec(15));

        this->skill_ = true;
        this->timeouts_.add_callback(g.clock().with_duration_sec(15),
                                     [](DecelerateMagician &self, GridRef) {
                                         self.skill_ = false;

                                         return false;
                                     });
    }
}
} // namespace core
} // namespace towerdefence