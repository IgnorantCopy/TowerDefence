#ifndef TOWERDEFENCE_CORE_ENTITY_ENTITY
#define TOWERDEFENCE_CORE_ENTITY_ENTITY

#include <cstdint>

namespace towerdefence {
namespace core {

struct GridRef;

struct AttackMixin {
    int32_t realized_attack = 0;
};

struct Entity {
    virtual void update(GridRef g) = 0;
    virtual ~Entity() = 0;
};

struct Defence {
    int32_t physics = 0;
    int32_t magic = 0;
};

struct EnemyInfo {
    int32_t health = 0;
    Defence defence;
    int32_t speed = 0;
};

struct Enemy : Entity, AttackMixin {
    virtual EnemyInfo info() const = 0;
};

enum class AttackType { Physics, Magic, Real };

struct TowerInfo {
    int32_t attack = 0;
    int32_t cost = 0;
    int32_t deploy_interval = 0;
    int32_t attack_interval = 0;
    AttackType attack_type;
};

struct Tower : Entity, AttackMixin {
    virtual TowerInfo info() const = 0;

    void increase_attack(int32_t atk) { realized_attack += atk; }
};

} // namespace core
} // namespace towerdefence

#endif