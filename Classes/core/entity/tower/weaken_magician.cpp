#include "weaken_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        void WeakenMagician::on_tick(GridRef g) {
            Tower::on_tick(g);
        }
    } // namespace core
} // namespace towerdefence