全部游戏状态都存储在 `Map` 中，主要有
- `grids` 存储了所有 `Grid`
- `shape` `Map` 的形状

`Grid` 是对地图上一个格子的抽象，其中有
- type，格子的类型
- enemies，存储了该格子中的所有敌人
- tower，存储了该格子中的塔

可以通过 `with_*` 接口访问 `Grid` 中的数据，它们均接收一个函数 `f`，若需要的数据存在则会调用 `f`。

可以通过 `remove_*` 移除 Grid 中的塔或敌人。

`Map` 的一个核心接口是 `update`，代表游戏时间流逝了 1 tick，`Map` 会递归调用 `Grid` 中实体的更新函数 (`on_tick`)，计算下一 tick 的状态。

所有塔共享一个基类 `Tower`，敌人共享 `Enemy`，`Tower` 和 `Enemy` 均继承 `Entity`, `Entity` 上有一些接口用于事件通知：
- `on_tick` 游戏时间流逝了 1 tick
- `on_hit` 该实体被打击
- `on_death` 该实体死亡
