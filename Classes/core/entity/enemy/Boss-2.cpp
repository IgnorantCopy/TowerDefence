#include "Boss-2.h"
#include "../../map.h"

namespace towerdefence::core {

Boss2::Boss2(id::Id id, route::Route route, const timer::Clock &clk)
    : Enemy(id, route), release_skill_({clk.with_period_sec(20), clk.with_period_sec(30),
                                        clk.with_period_sec(40)}) {
    this->add_buff_in({id, Buff::INVINCIBLE}, Buff::invincible(true),
                      clk.with_duration_sec(5));
}

void Boss2::on_tick(GridRef g) {
    Enemy::on_tick(g);

    this->timeouts_.on_tick(g.clock(), *this, g);

    auto add_buff_with_dur = [&clk = g.clock()](BuffIdentifier id, Buff b,
                                                uint32_t dur) {
        return [=, &clk](Tower &tower) {
            tower.add_buff_in(id, b, clk.with_duration_sec(dur));
        };
    };

    g.for_each_tower_on_trigger(
            release_skill_.dec_atk_spd,
            add_buff_with_dur({this->id, Buff::DECREASE_ATTACK_SPEED},
                              Buff::attack_speed(-30), 10));

    g.for_each_tower_on_trigger(
            release_skill_.silent,
            add_buff_with_dur({this->id, Buff::SILENT}, Buff::silent(true), 15));

    if (g.clock().is_triggered(release_skill_.withdraw)) {
        this->move_ = g.clock().never();
        this->release_skill_.silent = g.clock().never();
        this->release_skill_.dec_atk_spd = g.clock().never();
        if (auto &tower = g.get_nearest_tower(); tower.has_value()) {
            g.map.remove_tower((*tower)->id);
        }
        g.on_enemy_release_skill(*this, g.map, 0);
        this->timeouts_.add_callback(
            g.clock().with_duration(72), [](Boss2 &self, GridRef g) {
                self.reset_move_timer(g.clock());
                self.release_skill_.silent = g.clock().with_period_sec(30);
                self.release_skill_.dec_atk_spd = g.clock().with_period_sec(20);

                return false;

            });
    }

    if (g.clock().is_triggered(release_skill_.dec_atk_spd)) {
        this->move_ = g.clock().never();
        this->release_skill_.silent = g.clock().never();
        this->release_skill_.withdraw = g.clock().never();
        g.on_enemy_release_skill(*this, g.map, 10);
        this->timeouts_.add_callback(
            g.clock().with_duration(135), [](Boss2 &self, GridRef g) {
                self.reset_move_timer(g.clock());
                self.release_skill_.silent = g.clock().with_period_sec(30);
                self.release_skill_.withdraw = g.clock().with_period_sec(40);

                return false;

            });

    }

    if (g.clock().is_triggered(release_skill_.silent)) {
        this->move_ = g.clock().never();
        this->release_skill_.dec_atk_spd = g.clock().never();
        this->release_skill_.withdraw = g.clock().never();
        g.on_enemy_release_skill(*this, g.map, 15);
        this->timeouts_.add_callback(
            g.clock().with_duration(165), [](Boss2 &self, GridRef g) {
                self.reset_move_timer(g.clock());
                self.release_skill_.dec_atk_spd = g.clock().with_period_sec(20);
                self.release_skill_.withdraw = g.clock().with_period_sec(40);

                return false;

            });
    }
}
}// namespace towerdefence::core