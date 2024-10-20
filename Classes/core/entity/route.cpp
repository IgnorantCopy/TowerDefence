#include "route.h"

#include <iostream>

namespace towerdefence::core::route {
std::tuple<ssize, ssize> Route::next_direction() try {
    std::cout << std::format("{}", *this) << std::endl;
    auto [x, y] = diffs.at(pos);
    pos += 1;
    return {x, y};
} catch (const std::out_of_range &) {
    throw reached_end();
}
} // namespace towerdefence::core::route