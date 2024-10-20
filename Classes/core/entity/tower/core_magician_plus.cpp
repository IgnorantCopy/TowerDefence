#include "core_magician_plus.h"
#include "../../map.h"

namespace towerdefence {
namespace core {
CoreMagicianPlus::CoreMagicianPlus(id::Id id, const timer::Clock &clk)
    : Tower(id, clk), release_skill_(clk.with_period_sec(35)) {}

void CoreMagicianPlus::on_tick(GridRef g) {
    Tower::on_tick(g);
    this->timeouts_.on_tick(g.clock(), *this, g);

    auto &clk = g.clock();

    auto with_nearest = [&](auto &&f) {
        auto grids = g.with_radius(this->status().attack_radius_, linf_dis);
        if (auto it = grid_of_nearest_enemy(grids); it != grids.end()) {
            auto grid = *it;
            grid.with_nearest_enemy(f);
        }
    };

    if (clk.is_triggered(this->attack_)) {
        if (!this->skill_) {
            with_nearest([this, g](Enemy &e) {
                auto status = this->status();

                e.on_hit(status.attack_, status.attack_type_,g);
                g.on_enemy_attacked(e,*this);
            });
        } else {

            with_nearest([this, g](Enemy &e) {
                auto status = this->status();

                for (size_t i = 0; i < 4; ++i) {
                    e.on_hit(status.attack_, status.attack_type_, g);
                    g.on_enemy_attacked(e,*this);
                }
            });
        }
    }

    if (clk.is_triggered(release_skill_)&&!get_all_buff().silent_) {
        this->add_buff_in({this->id, Buff::DEFAULT},
                          Buff::attack_radius(1) & Buff::attack(3.30),
                          g.clock().with_duration_sec(10));

        this->skill_ = true;
        g.on_tower_release_skill(*this, g.map, 10);

        this->timeouts_.add_callback(g.clock().with_duration_sec(10),
                                     [](CoreMagicianPlus &self, GridRef) {
                                         self.skill_ = false;
                                         return false;
                                     });
    }
}

} // namespace core
} // namespace towerdefence