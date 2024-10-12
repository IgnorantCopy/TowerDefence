#include "special_magician.h"
#include "../../map.h"
#include <algorithm>

namespace towerdefence {
namespace core {
void SpecialMagician::on_tick(GridRef g) {
    Tower::on_tick(g);

    auto &clk = g.clock();
    auto status = this->status();

    if (clk.is_triggered(this->attack_)) {
        auto grids = g.with_radius(status.attack_radius_, linf_dis);
        if (auto it = grid_of_nearest_enemy(grids); it != grids.end()) {
            auto grid = *it;
            if (this->has_skill) {
                grid.attack_enemies_in_radius(
                    status.with_attack_radius(1).with_attack(status.attack_ *
                                                             1.5),
                    linf_dis);
                this->has_skill = false;
            } else {
                single_attack(*this, grid);
            }
            this->is_attacking = true;
            this->silence_count = 0;
        } else {
            this->is_attacking = false;
        }
    }

    if (clk.is_triggered(this->update_silence_count) && !this->is_attacking) {
        this->silence_count = std::min(this->silence_count + 1, 3zu);
    }

    if (clk.is_triggered(this->update_silence_count)) {
        this->has_skill = true;
    }
}
} // namespace core
} // namespace towerdefence