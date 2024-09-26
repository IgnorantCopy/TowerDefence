#include "Not-attacked.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {

        void NotAttacked::on_tick(GridRef g) {}

        void NotAttacked::on_hit(GridRef g) {
            this->add_buff(this->id, {.invincible = true, .not_hit = false});
        }

    } // namespace core
} // namespace towerdefence