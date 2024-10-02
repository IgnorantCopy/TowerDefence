#ifndef TOWERDEFENCE_CORE_ID
#define TOWERDEFENCE_CORE_ID

#include <cstdint>
namespace towerdefence::core::id {

struct Id {
    uint32_t v;
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

#endif
