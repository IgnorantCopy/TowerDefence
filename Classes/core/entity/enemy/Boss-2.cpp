#include "Boss-2.h"
#include "../../map.h"
#include <stdexcept>

namespace towerdefence {
    namespace core {

        Boss2::Boss2(id::Id id, const timer::Clock &clk)
                : Enemy(id), release_skill_(clk.with_period_sec(20)) {}

        void Boss2::on_tick(GridRef g) {
            this->update_buff(g.clock());

            if (g.clock().is_triggered(release_skill_)) {
                for (auto &grid : g.map.grids) {
                    grid.with_tower(
                            [this, &clk = g.clock()](std::unique_ptr<Tower> &tower) {
                                tower->add_buff_in({this->id, Buff::DEFAULT},
                                                   Buff::attack_speed(-50) & Buff::silent(true),
                                                   clk.with_duration_sec(10));
                                has_buff_.insert(tower->id);
                            });
                }
            }
        }

        void Boss2::on_death(GridRef g) {
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