#include "core_magician.h"
#include "../../map.h"

namespace towerdefence {
    namespace core {
        void CoreMagician::on_tick(GridRef g) {
            Tower::on_tick(g);
        }
    } // namespace core
} // namespace towerdefence