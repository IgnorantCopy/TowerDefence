#ifndef TOWERDEFENCE_CORE_ENTITY_ENTITY
#define TOWERDEFENCE_CORE_ENTITY_ENTITY

#include <any>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <numeric>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

#include "../id.h"
#include "../timer.h"
#include "route.h"

namespace towerdefence {
namespace core {

struct GridRef;

struct Defence {
    int32_t physics_ = 0;
    int32_t magic_ = 0;

    constexpr Defence(int32_t physics, int32_t magic)
        : physics_(physics), magic_(magic) {}

    Defence operator+(const Defence &rhs) const {
        return Defence(physics_ + rhs.physics_, magic_ + rhs.magic_);
    }

    Defence &operator+=(const Defence &rhs) {
        *this = *this + rhs;
        return *this;
    }
};

struct AttackMixin {
    int32_t realized_attack_ = 0;
};

struct NormalAttackMixin {
    timer::Timer attack_ = timer::Timer::never();
};

#define BUFF_CONSTRUCTOR(type, name)                                           \
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
    double real_attack_ = 0;
    // actual_defence = base_defence + defence_correction_
    Defence defence_correction_{0, 0};
    // actual_attack_radius = base_attack_radius + attack_radius
    int32_t attack_radius_ = 0;
    bool invincible_ = false;
    bool silent_ = false;
    bool attack_stop_ = false;

    BUFF_CONSTRUCTOR(int32_t, attack_speed)
    BUFF_CONSTRUCTOR(double, speed)
    BUFF_CONSTRUCTOR(double, attack)
    BUFF_CONSTRUCTOR(double, real_attack)
    BUFF_CONSTRUCTOR(Defence, defence_correction)
    BUFF_CONSTRUCTOR(int32_t, attack_radius)
    BUFF_CONSTRUCTOR(bool, invincible)
    BUFF_CONSTRUCTOR(bool, silent)
    BUFF_CONSTRUCTOR(bool, attack_stop)

    static constexpr uint32_t DEFAULT = 0;
    static constexpr uint32_t INVINCIBLE = 1;
    static constexpr uint32_t DECREASE_SPEED = 1 << 1;
    static constexpr uint32_t DECREASE_ATTACK_SPEED = 1 << 2;
    static constexpr uint32_t SILENT = 1 << 3;

    constexpr Buff() = default;
    constexpr Buff(int32_t attack_speed, double speed, double attack,
                   double real_attack, Defence defence_correction,
                   int32_t attack_radius, bool invincible, bool silent,
                   bool attack_stop)
        : attack_speed_(attack_speed), speed_(speed), attack_(attack),
          real_attack_(real_attack), defence_correction_(defence_correction),
          attack_radius_(attack_radius), invincible_(invincible),
          silent_(silent), attack_stop_(attack_stop) {}

    Buff operator&(const Buff &rhs) const {
        return Buff(attack_speed_ + rhs.attack_speed_, speed_ + rhs.speed_,
                    attack_ + rhs.attack_, real_attack_ + rhs.real_attack_,
                    defence_correction_ + rhs.defence_correction_,
                    attack_radius_ + rhs.attack_radius_,
                    invincible_ || rhs.invincible_, silent_ || rhs.silent_,
                    attack_stop_ || rhs.attack_stop_);
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
    constexpr BuffIdentifier(id::Id entity_id, uint32_t buff_id)
        : id_((uint64_t(entity_id.v) << 32) | uint64_t(buff_id)) {}

    bool operator==(const BuffIdentifier &rhs) const { return id_ == rhs.id_; }

    bool is_from(id::Id id) const { return id_ >> 32 == id.v; }
};

struct IdMixin {
    id::Id id;
};

struct BuffMixin {
    std::unordered_map<BuffIdentifier,
                       std::pair<Buff, std::optional<timer::Timer>>,
                       BuffIdentifier::hasher>
        buffs;

    using Entry = decltype(buffs)::value_type;

    void add_buff(BuffIdentifier id, Buff b) { buffs.insert({id, {b, {}}}); }
    void add_buff_in(BuffIdentifier id, Buff b, timer::Timer t) {
        buffs.insert({id, {b, t}});
    }

    void remove_buff_from(id::Id id) {
        std::erase_if(this->buffs, [id](const Entry &item) {
            return item.first.is_from(id);
        });
    }

    Buff get_all_buff() const {
        return std::accumulate(buffs.cbegin(), buffs.cend(), Buff{},
                               [](const Buff acc, const Entry &val) {
                                   return acc & val.second.first;
                               });
    }

    void update_buff(const timer::Clock &clk) {
        std::erase_if(this->buffs, [&clk](const Entry &item) {
            const auto &[buff, timer] = item.second;

            return timer
                .transform([clk](const timer::Timer &timer) {
                    return clk.is_triggered(timer);
                })
                .value_or(false);
        });
    }
};

struct Entity {
    // called on each tick
    virtual void on_tick(GridRef g);
    // called when the entity dies
    virtual void on_death(GridRef g);

    virtual ~Entity() {};
};

struct RouteMixin {
    route::Route route_;

    route::Route &route() { return this->route_; }
    const route::Route &route() const { return this->route_; }
};

struct MoveMixin {
    double move_progress_ = 0;
};

struct ExtraStorage {
    std::unordered_map<std::string, std::any> storage_;

    // get storage of key
    //
    // # Throws
    //
    // `std::bad_any_cast` if type mismatch.
    //
    // `std::out_of_range` if key does not exist.
    template <class T> T get_storage(const std::string &key) const {
        auto &value = this->storage_.at(key);

        return std::any_cast<T>(value);
    }

    // set storage of key
    template <class T> void set_storage(const std::string &key, T value) {
        this->storage_[key] = value;
    }
};

enum class AttackType { Physics, Magic, Real };
enum class TowerType {
    ArcherBase,
    HighspeedArcher,
    HighspeedArcherPlus,
    Bomber,
    BomberPlus,
    Archer,
    ArcherPlus,
    MagicianBase,
    CoreMagician,
    CoreMagicianPlus,
    DiffusiveMagician,
    DiffusiveMagicianPlus,
    SpecialMagician,
    SpecialMagicianPlus,
    HelperBase,
    DecelerateMagician,
    DecelerateMagicianPlus,
    WeakenMagician,
    WeakenMagicianPlus,
    AggressiveMagician,
    AggressiveMagicianPlus
};
enum class EnemyType {
    Worm,
    Dog,
    Soldier,
    Warlock,
    Destroyer,
    Tank,
    Crab,
    SpeedUp,
    AttackDown,
    LifeUp,
    NotAttacked,
    Boss1,
    Boss2
};

struct EnemyInfo {
    int32_t health_ = 0;
    Defence defence_;
    int32_t speed_ = 0;
    EnemyType enemy_type_;
    size_t total_frames_ = 0;
    size_t current_frame_ = 0;

    constexpr EnemyInfo(int32_t health, Defence defence, int32_t speed,
                        EnemyType enemy_type, size_t total_frames)
        : health_(health), defence_(defence), speed_(speed),
          enemy_type_(enemy_type), total_frames_(total_frames) {}
};

struct Enemy : Entity,
               AttackMixin,
               BuffMixin,
               IdMixin,
               RouteMixin,
               MoveMixin,
               ExtraStorage {
    Enemy(id::Id id, route::Route route)
        : IdMixin{id}, RouteMixin{route} {}

    virtual EnemyInfo info() const = 0;

    size_t remaining_distance() const {
        return this->route_.remaining_distance();
    }

    // Returns current progress of moving as a value in [0, 1]
    double move_progress() const {
        assert(this->move_progress_ >= 0 && this->move_progress_ <= 1);
        return this->move_progress_;
    }

    // Calculates current defence and speed that takes buffs into account
    // and current health_ = info().health_ - realized_attack
    virtual EnemyInfo status() const {
        auto base = info();
        auto buffs = get_all_buff();

        base.health_ -= realized_attack_;
        base.defence_ += buffs.defence_correction_;
        base.speed_ = base.speed_ * (1 + buffs.speed_);

        return base;
    }

    void on_tick(GridRef g) override;
    void on_death(GridRef g) override;

    virtual void on_hit(int32_t atk, AttackType attack_type, GridRef g) ;
};

struct TowerInfo {
    int32_t attack_ = 0;
    int32_t cost_ = 0;
    int32_t deploy_interval_ = 0;
    double attack_interval_ = 0; // actual_attack_attack_speed
    size_t attack_radius_ = 0;
    AttackType attack_type_;
    TowerType tower_type_;

    constexpr TowerInfo(int32_t attack, int32_t cost, int32_t deploy_interval,
                        double attack_interval, int32_t attack_radius,
                        AttackType attack_type, TowerType tower_type)
        : attack_(attack), cost_(cost), deploy_interval_(deploy_interval),
          attack_interval_(attack_interval), attack_radius_(attack_radius),
          attack_type_(attack_type), tower_type_(tower_type) {}

    TowerInfo with_attack_radius(size_t r) const noexcept {
        auto copied = *this;
        copied.attack_radius_ = r;
        return copied;
    }

    TowerInfo with_attack(int32_t a) const noexcept {
        auto copied = *this;
        copied.attack_ = a;
        return copied;
    }
};

struct Tower : Entity, AttackMixin, BuffMixin, IdMixin, NormalAttackMixin {
    Tower(id::Id id, const timer::Clock &clk) : IdMixin{id} {}

    Tower(Tower &&) = delete;

    virtual TowerInfo info() const = 0;

    virtual TowerInfo status() const {
        auto base = info();
        auto buffs = get_all_buff();

        base.attack_ += buffs.attack_;
        base.attack_interval_ =
            base.attack_interval_ / (double(100 + buffs.attack_speed_) / 100);
        base.attack_radius_ += buffs.attack_radius_;

        return base;
    }

    void on_tick(GridRef g) override;

    void reset_attack_timer(const timer::Clock &clk) {
        this->attack_ = clk.with_period_sec(this->status().attack_interval_);
    }
};

template <class Self, class G = GridRef, class... Args> struct TimeOutMixin {
    timer::CallbackTimer<Self &, G, Args...> timeouts_;

    void stop_timer_for(timer::Timer &timer, uint32_t d,
                        const timer::Clock &clk) {
        timer = clk.never();
        this->timeouts_.add_callback(clk.with_duration_sec(d),
                                     [&](Self &self, G g, Args...) {
                                         self.reset_attack_timer(g.clock());
                                         return false;
                                     });
    }
};

// used in CallbackTimer
bool restore_normal_attack(Tower &self, GridRef g);

[[deprecated("use grid_of_nearest_enemy instead")]] auto
get_enemy_grid(Tower &, std::vector<GridRef> &)
    -> std::vector<GridRef>::iterator;

auto grid_of_nearest_enemy(std::vector<GridRef> &grids)
    -> std::vector<GridRef>::iterator;

void single_attack(Tower &, GridRef);

struct Map;

struct EnemyFactoryBase {
    virtual std::unique_ptr<Enemy> construct(id::Id id,
                                             const timer::Clock &clk) = 0;
};

template <class T> struct EnemyFactory final : EnemyFactoryBase {
    route::Route route;
    std::unordered_map<std::string, std::any> extra_storage;

    std::unique_ptr<Enemy> construct(id::Id id,
                                     const timer::Clock &clk) override;

    EnemyFactory(route::Route route_) : EnemyFactory(route_, {}) {}
    EnemyFactory(route::Route route_,
                 std::unordered_map<std::string, std::any> extra_storage_)
        : route(std::move(route_)), extra_storage(std::move(extra_storage_)) {}
};

template <class T>
std::unique_ptr<Enemy> EnemyFactory<T>::construct(id::Id id,
                                                  const timer::Clock &clk) {
    std::unique_ptr<Enemy> res;
    if constexpr (std::is_constructible_v<T, id::Id, route::Route,
                                          const timer::Clock &>) {
        res = std::make_unique<T>(id, route, clk);
    } else if constexpr (std::is_constructible_v<T, id::Id, route::Route>) {
        res = std::make_unique<T>(id, route);
    } else {
        static_assert(false, "Unsupported type");
    }

    res->storage_ = extra_storage;

    return res;
}

struct TowerFactoryBase {
    virtual std::unique_ptr<Tower> construct(id::Id id,
                                             const timer::Clock &clk) = 0;
    virtual TowerInfo info() const = 0;

    virtual ~TowerFactoryBase() = default;
};

template <class T> struct TowerFactory : TowerFactoryBase {
    std::unique_ptr<Tower> construct(id::Id id,
                                     const timer::Clock &clk) override;
    TowerInfo info() const override;
};

template <class T>
std::unique_ptr<Tower> TowerFactory<T>::construct(id::Id id,
                                                  const timer::Clock &clk) {
    std::unique_ptr<Tower> res;
    if constexpr (std::is_constructible_v<T, id::Id, const timer::Clock &>) {
        res = std::make_unique<T>(id, clk);
    } else if constexpr (std::is_constructible_v<T, id::Id>) {
        res = std::make_unique<T>(id);
    } else {
        static_assert(false, "Unsupported type");
    }

    res->reset_attack_timer(clk);

    return res;
}

template <class T> TowerInfo TowerFactory<T>::info() const { return T::INFO; }

} // namespace core
} // namespace towerdefence

#endif