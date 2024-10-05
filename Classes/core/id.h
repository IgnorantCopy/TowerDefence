#ifndef TOWERDEFENCE_CORE_ID
#define TOWERDEFENCE_CORE_ID

#include <cstddef>
#include <cstdint>
#include <functional>
namespace towerdefence::core::id {

struct Id {
    uint32_t v;

    bool operator==(const Id &rhs) const noexcept { return v == rhs.v; }
};

struct IdGenerator {
    uint32_t cur = 0;

    constexpr explicit IdGenerator() = default;

    Id gen() {
        cur += 1;
        return {cur};
    }
};

} // namespace towerdefence::core::id

template <> struct std::hash<towerdefence::core::id::Id> {
    size_t operator()(const towerdefence::core::id::Id &id) const noexcept {
        return std::hash<uint32_t>{}(id.v);
    }
};

#endif
