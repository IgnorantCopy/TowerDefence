#include "aggressive_magician.h"
#include "../../map.h"
#include <stdexcept>

namespace towerdefence {
    namespace core {
        AggressiveMagician::AggressiveMagician(id::Id id, const timer::Clock &clk)
                : Tower(id), release_skill_(clk.with_period_sec(70)),
                  attack_(clk.with_period_sec(INFO.attack_interval_)) {}
        void AggressiveMagician::on_tick(GridRef g) {
            Tower::on_tick(g);
            auto status = this->status();
            auto &clk = g.clock();
            if (clk.is_triggered(this->attack_)){
                this->attack_ = clk.never();
                for (auto grid : g.with_radius(status.attack_radius_, linf_dis)) {
                    grid.current().with_tower(
                            [this, &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                                tower->add_buff({this->id, Buff::DEFAULT},
                                                Buff::attack(0.20));
                            });
                }
            }
            if (clk.is_triggered(release_skill_)) {
                this->release_skill_ = clk.never(); // stop release_skill
                for (auto grid : g.with_radius(status.attack_radius_, linf_dis)) {
                    grid.current().with_tower(
                            [this, &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                                tower->add_buff({this->id, Buff::DEFAULT},
                                                Buff::attack(0.50));
                            });
                }
            }
        }

        void AggressiveMagician::on_death(GridRef g) {
            for (auto tower_id : has_buff_) {
                try {
                    auto &tower = g.map.get_tower_by_id(tower_id);
                    tower.remove_buff_from(id);
                } catch (const std::out_of_range &) {
                }
            }
        }
    } // namespace core
} // namespace towerdefence