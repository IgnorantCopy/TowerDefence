#include <cstdint>
namespace towerdefence::core::id {

struct IdGenerator {
    uint32_t cur = 0;

    constexpr explicit IdGenerator() = default;

    uint32_t gen() {
        cur += 1;
        return cur;
    }
};

} // namespace towerdefence::core::id