#include "map.h"

namespace towerdefence {
namespace core {
GridRef Map::get_ref(size_t row, size_t column) {
    return GridRef(*this, row, column);
}
} // namespace core
} // namespace towerdefence
