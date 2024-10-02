#ifndef TOWERDEFENCE_CORE_ENTITY_ENTITY
#define TOWERDEFENCE_CORE_ENTITY_ENTITY

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <unordered_map>

#include "../timer.h"

namespace towerdefence {
namespace core {

struct GridRef;

struct AttackMixin {
    int32_t realized_attack = 0;
};

#define BUFF_CONSTUCTOR(type, name)                                            \
    static constexpr Buff name(type name) {                                    \
        Buff b;                                                                \
        b.name##_ = name;                                                      \
        return b;                                                              \
    }

struct Buff {
    // actual_attack_speed = base_attack_speed + attack_speed
    int32_t attack_speed_ = 0;
    // actual_speed = base_speed * (1 + speed)
    double speed_ = 0;
    // actual_attack = base_attack * (1 + attack)
    double attack_ = 0;
    bool invincible_ = false;
    bool silent_ = false;

    std::optional<timer::Timer> timer_;

    BUFF_CONSTUCTOR(int32_t, attack_speed)
    BUFF_CONSTUCTOR(double, speed)
    BUFF_CONSTUCTOR(double, attack)
    BUFF_CONSTUCTOR(bool, invincible)
    BUFF_CONSTUCTOR(bool, silent)

    static constexpr uint32_t DEFAULT = 0;
    static constexpr uint32_t INVINCIBLE = 1;
    static constexpr uint32_t DECREASE_SPEED = 2;

    constexpr Buff() = default;
    constexpr Buff(int32_t attack_speed, double speed, double attack,
                   bool invincible, bool silent)
        : attack_speed_(attack_speed), speed_(speed), attack_(attack),
          invincible_(invincible), silent_(silent) {}

    Buff operator&(const Buff &rhs) const {
        return Buff(attack_speed_ + rhs.attack_speed_, speed_ + rhs.speed_,
                    attack_ + rhs.attack_, invincible_ || rhs.invincible_,
                    silent_ || rhs.silent_);
    }

    bool should_expire(const timer::Clock &clock) const {
        return timer_.has_value() && clock.is_triggered(*timer_);
    }

    constexpr Buff with_timer(timer::Timer timer) const {
        Buff b = *this;
        b.timer_ = {timer};
        return b;
    }
};

// Uniquely indentify a buff.
struct BuffIdentifier {
    struct hasher {
        std::size_t operator()(const BuffIdentifier &s) const noexcept {
            return std::hash<uint64_t>{}(s.id_);
        }
    };

    uint64_t id_;

    // entity_id uniquely identifies an entity
    // buff_id uniquely identifies a buff the entity may give
    constexpr BuffIdentifier(uint32_t entity_id, uint32_t buff_id)
        : id_((uint64_t(entity_id) << 32) | uint64_t(buff_id)) {}

    bool operator==(const BuffIdentifier &rhs) const { return id_ == rhs.id_; }
};

struct IdMixin {
    uint32_t id;
};

struct BuffMixin {
    std::unordered_map<BuffIdentifier, Buff, BuffIdentifier::hasher> buffs;

    void add_buff(BuffIdentifier id, Buff b) { buffs.insert({id, b}); }
    void add_buff_in(BuffIdentifier id, Buff b, timer::Timer t) {
        buffs.insert({id, b.with_timer(t)});
    }
    void update_buff(const timer::Clock &clk) {
        for (auto it = buffs.cbegin(); it != buffs.cend(); ++it) {
            if (it->second.should_expire(clk)) {
                buffs.erase(it);
            }
        }
    }
};

struct Entity {
    // called on each tick
    virtual void on_tick(GridRef g);
    // called when the entity dies
    virtual void on_death(GridRef g);
    // called when entity is hit
    virtual void on_hit(GridRef g);

    virtual ~Entity() = 0;
};

struct Defence {
    int32_t physics_ = 0;
    int32_t magic_ = 0;

    constexpr Defence(int32_t physics, int32_t magic)
        : physics_(physics), magic_(magic) {}
};

struct EnemyInfo {
    int32_t health_ = 0;
    Defence defence_;
    int32_t speed_ = 0;

    constexpr EnemyInfo(int32_t health, Defence defence, int32_t speed)
        : health_(health), defence_(defence), speed_(speed) {}
};

struct Enemy : Entity, AttackMixin, BuffMixin, IdMixin {
    virtual EnemyInfo info() const = 0;
};

enum class AttackType { Physics, Magic, Real };

struct TowerInfo {
    int32_t attack_ = 0;
    int32_t cost_ = 0;
    int32_t deploy_interval_ = 0;
    int32_t attack_interval_ = 0; // actual_attack_attack_speed
    AttackType attack_type_;

    constexpr TowerInfo(int32_t attack, int32_t cost, int32_t deploy_interval,
                        int32_t attack_interval, AttackType attack_type)
        : attack_(attack), cost_(cost), deploy_interval_(deploy_interval),
          attack_interval_(attack_interval), attack_type_(attack_type) {}
};

struct Tower : Entity, AttackMixin, BuffMixin, IdMixin {
    virtual TowerInfo info() const = 0;

    void increase_attack(int32_t atk) { realized_attack += atk; }
};

} // namespace core
} // namespace towerdefence

#endif