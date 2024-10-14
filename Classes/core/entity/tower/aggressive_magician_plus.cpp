#include "aggressive_magician_plus.h"
#include "../../map.h"
#include <stdexcept>

namespace towerdefence {
    namespace core {
        AggressiveMagicianPlus::AggressiveMagicianPlus(id::Id id, const timer::Clock &clk)
                : Tower(id, clk), release_skill_(clk.with_period_sec(40)) {}
        void AggressiveMagicianPlus::on_tick(GridRef g) {
            Tower::on_tick(g);
            auto status = this->status();
            auto &clk = g.clock();
            if (clk.is_triggered(this->attack_)){
                for (auto grid : g.with_radius(status.attack_radius_, linf_dis)) {
                    grid.current().with_tower(
                            [this, &clk](std::unique_ptr<Tower> &tower) {

                                tower->add_buff({this->id, Buff::DEFAULT},
                                                Buff::attack(0.20));
                            });
                }
            }
            if (clk.is_triggered(release_skill_)&&!get_all_buff().silent_) {
                for (auto grid : g.with_radius(status.attack_radius_, linf_dis)) {
                    grid.current().with_tower(
                            [this, &clk](std::unique_ptr<Tower> &tower) {

                                tower->add_buff({this->id, Buff::DEFAULT},
                                                Buff::attack(0.50)&Buff::attack_speed(50));
                            });
                }
            }
        }

        void AggressiveMagicianPlus::on_death(GridRef g) {
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