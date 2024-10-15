#include "decelerate_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        DecelerateMagicianPlus::DecelerateMagicianPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(60)) {}

        void DecelerateMagicianPlus::on_tick(GridRef g) {
            Tower::on_tick(g);

            auto &clk = g.clock();

            if (clk.is_triggered(this->attack_)) {
                auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
                if (auto it = grid_of_nearest_enemy(grids); it != grids.end()) {
                    auto grid = *it;
                    grid.with_nearest_enemy([this, &clk, g](Enemy &e) {
                        auto status = this->status();

                        e.on_hit(status.attack_, status.attack_type_, g);
                        e.add_buff_in({this->id, Buff::DECREASE_SPEED},
                                      Buff::speed(-40),
                                      clk.with_duration_sec(2));
                    });
                }
            }

            if (clk.is_triggered(release_skill_)&&!get_all_buff().silent_) {
                this->add_buff_in({this->id, Buff::DEFAULT},
                                  Buff::attack_radius(1),
                                  g.clock().with_duration_sec(35));
                auto status = this->status();
                for (auto ref : g.with_radius(status.attack_radius_, linf_dis)) {
                    ref.grid.with_enemy([this, clk](Enemy &enemy) {
                        enemy.add_buff_in({this->id, Buff::DECREASE_SPEED},
                                          Buff::speed(-80),
                                          clk.with_duration_sec(35));
                    });
                }

                g.on_tower_release_skill(*this, g.map, 35);

                this->stop_timer_for(this->attack_, 35, clk);
            }
        }
    } // namespace core
} // namespace towerdefence