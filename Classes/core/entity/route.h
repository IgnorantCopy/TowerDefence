#ifndef TOWERDEFENCE_CORE_ENTITY_ROUTE
#define TOWERDEFENCE_CORE_ENTITY_ROUTE

#include <cstddef>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace towerdefence::core::route {

using ssize = std::make_signed_t<size_t>;

struct reached_end : std::out_of_range {
    reached_end() : std::out_of_range("A route reached end") {}
};

struct Route {
    std::vector<std::pair<ssize, ssize>> diffs;
    size_t pos = 0;

    explicit Route() {}

    explicit Route(std::vector<std::pair<ssize, ssize>> diffs) : diffs(std::move(diffs)) {}

    // Returns a tuple of (x, y),
    // which indicates how far the entity should move in x(row)
    // and y(column) axis.
    //
    // If no further routes, `std::out_of_range` is thrown.
    //
    // # Example
    //
    // Route route = /* init */;
    // auto [x, y] = route.next_direction();
    std::tuple<ssize, ssize> next_direction() try {
        auto [x, y] = diffs.at(pos);
        pos += 1;
        return {x, y};
    } catch (const std::out_of_range &) {
        throw reached_end();
    }

    size_t remaining_distance() const noexcept { return diffs.size() - pos; }
};

} // namespace towerdefence::core::route

#endif
