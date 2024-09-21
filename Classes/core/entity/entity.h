#ifndef TOWERDEFENCE_CORE_ENTITY_ENTITY
#define TOWERDEFENCE_CORE_ENTITY_ENTITY

#include <cstdint>

namespace towerdefence {
namespace core {
struct Defence {
    int32_t physics;
    int32_t magic;
};

struct EnemyInfo {
    int32_t health;
    Defence defence;
    int32_t speed;
};

struct Enemy {
    virtual EnemyInfo info() const = 0;
};

enum class AttackType { Physics, Magic, Real };

struct TowerInfo {
    int32_t attack;
    int32_t cost;
    int32_t deploy_interval;
};

struct Tower {
    virtual TowerInfo info() const = 0;
};

} // namespace core
} // namespace towerdefence

#endif