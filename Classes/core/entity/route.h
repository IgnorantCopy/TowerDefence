#ifndef TOWERDEFENCE_CORE_ENTITY_ROUTE
#define TOWERDEFENCE_CORE_ENTITY_ROUTE

#include <cstddef>
#include <format>
#include <sstream>
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
    std::tuple<ssize, ssize> next_direction();

    size_t remaining_distance() const noexcept { return diffs.size() - pos; }
};

} // namespace towerdefence::core::route

template<>
struct std::formatter<towerdefence::core::route::Route>
{ 
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
       return ctx.begin();
    }
 
    template<class FmtContext>
    FmtContext::iterator format(towerdefence::core::route::Route& r, FmtContext& ctx) const
    {
       std::ostringstream out;
       out << std::format("Route @ {} / {} : ", r.pos, r.diffs.size());
    
        for (auto [i, j] : r.diffs) {
            out << std::format("({}, {}), ", i, j);
        }

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

#endif
