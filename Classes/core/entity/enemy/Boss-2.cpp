#include "Boss-2.h"
#include "../../map.h"

namespace towerdefence::core {

Boss2::Boss2(id::Id id, route::Route route, const timer::Clock &clk)
    : Enemy(id, route), release_skill_({clk.with_period_sec(20), clk.with_period_sec(30),
                                        clk.with_period_sec(40)}) {
    this->add_buff_in({id, Buff::INVINCIBLE}, Buff::invincible(true),
                      clk.with_duration_sec(20));
}

void Boss2::on_tick(GridRef g) {
    Enemy::on_tick(g);

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
        if (auto &tower = g.get_nearest_tower(); tower.has_value()) {
            g.map.remove_tower((*tower)->id);
        }
        g.on_enemy_release_skill(*this, g.map, 0);
    }

    if (g.clock().is_triggered(release_skill_.dec_atk_spd)) {
        g.on_enemy_release_skill(*this, g.map, 10);
    }

    if (g.clock().is_triggered(release_skill_.silent)) {
        g.on_enemy_release_skill(*this, g.map, 15);
    }
}
}// namespace towerdefence::core