# 1. 项目简介

本项目是基于 cocos2d-x 引擎实现的塔防游戏。

# 2. 实现的功能及其实现方法

游戏分为前端的展示部分和后端的逻辑部分。

## 2.1 游戏逻辑

游戏逻辑全部实现于 `Classes/core` (cmake target: core) 中。

### Map

`Map` 类型封装了一局游戏的全部状态，包括地图、时钟、生命值、实体、`id` 分配器。地图在逻辑上是一个矩形，被分为若干个格子，每个格子中可为空、塔 (`Tower`) 或若干个敌人 (`Enemy`)，以类型 `Grid` 表示。此外 `GridRef` 类型代表了对地图中某一个的引用，为游戏中实体 (`Entity`) 访问上下文提供了便利的接口。

`Map` 向游戏的前端部分提供了一系列回调 (`Map::on_*`)，用于在特定事件发生时通知前端。

`Map` 提供了一系列接口，用于操作地图状态，如添加/移除塔和敌人。

特别的，`Map::update` 是游戏状态更新的入口点，其会更新时钟状态，然后依次更新地图每一格上 `Entity` 的状态，使敌人根据预设路线移动，最后结算攻击，移除死亡的 `Entity`。前端通过定时调用此方法驱动游戏进行。

### Entity

`Entity` 是对游戏中实体对象的抽象，其派生出 `Tower` 和 `Enemy` 两个虚基类分别抽象了塔和敌人两方。

每个 `Entity` 都有与之对应的唯一 `id` 作为标识。`id` 分配由 `Classes/core/id.h` 中的 `IdGenerator` 完成，每次通过自增生成一个不重复的 `id`。

`Entity` 定义了两个虚方法 `on_tick` 和 `on_death` 分别在时钟更新和死亡时被调用。

`Enemy` 额外定义了一个虚方法 `on_hit` 在被攻击时被调用。

此外，`*Mixin` 类型为 `Tower` 和 `Enemy` 提供了可选的额外状态和功能，如 buff、生命值等。

#### Buff

buff 功能由 `BuffMixin` 实现，其存储了一个实体当前的 `Buff`，并在更新时移除过期的 `Buff`，其提供了添加、移除 `buff` 和计算 `buff` 总效果的接口。

为了处理 `buff` 重复施加的问题，一个 buff 由 `BuffIdentifier` 和 `Buff` 构成，`Buff` 存储 `buff` 本身的信息，如种类。`BuffIdentifier` 由施加该 `buff` 的 `Entity` 的 `id` 和 `buff_id` 组成，其中 `buff_id` 表示该 `Entity` 对该 buff 的编号。由此，实现当同一个 `Entity` 多次施加同一个 buff 时，会重置该 buff 的时间而不会叠加效果。

---

在 `Classes/core/entity/tower` 和 `Classes/core/entity/enemy` 中分别是派生自 `Tower` 和 `Enemy` 的具体塔和敌人类型。这些类型各自可能存有自己的状态。

通过模版提供了相应的工厂 (`TowerFactory<T>` 和 `EnemyFactory<T>`)，实现构造函数的分派 (一些类需要额外的上下文进行构造而另一些不需要)和初始化。

---

### 时钟

游戏逻辑采用基于游戏刻 (tick) 进行计时，从而使各个事件的发生更加稳定。时钟实现在 `Classes/core/timer.h` 中，分为 `CLock` 和 `Timer` 两部分。`Clock` 存储时间状态，`Timer` 是无状态的，表示一个时间事件触发的条件。`Clock::is_triggered` 用于查询一个 `Timer` 对应的条件是否达到。

`CallbackTimer` 在此基础上实现了在达到 `Timer` 条件时调用回调的功能。


## 2.3 动画效果

……

## 2.4 存档

……

## 2.5 剧情

……

## 2.6 项目管理

项目使用 git 进行版本控制，大多数更改在各自分支上开发，通过 pull request 进行 code review 后合入主分支。

项目使用 github-actions 实现 CI 功能。每次 push 和 pull request 是会编译当前代码并上传构建结果，防止有编译问题的代码合入主分支影响开发。此外，还有一个手动触发的 action 用于 release 构建。

项目使用 cmake 作为构建系统。

项目主要支持的平台是 windows，但由于有成员使用 linux 开发，故也可在 linux 上运行。项目带有一个 nix flake 实现 linux 上的可复现构建，使用 `nix run github:IgnorantCopy/TowerDefence#tower-defence` 即可运行。

# 3. 组员及分工

