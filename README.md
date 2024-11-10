# 1. 项目简介

本项目是基于cocos2d-x引擎实现的塔防游戏……

# 2. 实现的功能及其实现方法

## 2.1 计时器

……

## 2.2 Buff

……

## 2.3 动画效果

……

## 2.4 存档

保存存档的功能主要是利用cocos2d-x引擎提供的UserDefault类，将游戏中的数据保存到本地文件中。 
具体实现方法如下：
```
// 保存存档
auto userDefault = UserDefault::getInstance();
// 两个参数分别是key和对应的value
userDefault->setBoolForKey("clearItemShow", false); // 设置布尔值
userDefault->setIntegerForKey("level1", 0); // 设置整数值
// 类似的，还有setStringForKey()、setFloatForKey()等方法
```
```
// 读取存档
auto userDefault = UserDefault::getInstance();
// 两个参数分别是key和如果未找到对应的value则返回的默认值
int level1Scene = userDefault->getIntegerForKey("level1", 0);
```
存档能够保证每次进入游戏时，关卡分数、关卡解锁和音量调整等都是上一次退出时的状态，提高了用户体验。
存档会在用户通关游戏关卡或者改变音量时自动保存，不需要用户手动保存，确保游戏存档数据及时更新。
此外，在游戏的Manual中的Setting界面，用户可以点击Clear Saved Data按钮来清除存档数据，重新开始游戏。这时游戏就会回到初始状态，关卡分数、关卡解锁和音量调整等都会被重置。
值得注意的是，在游玩关卡过程中，通过塔的查询按钮是可以进入Manual界面的，但此时Setting中只有音量调节选项，没有Clear Saved Data按钮，因为此时游戏还在进行中，不应该清除存档数据。

**展示如下：**
刚进游戏时，一切都是初始
![SavedData1](README.assets/SavedData1.png){width=600}
游玩后，关卡分数、关卡解锁和音量调整等都会被保存，退出后再次进入游戏时游戏进度与退出前一致：
![SavedData2](README.assets/SavedData2.png){width=600}
点击Clear Saved Data按钮后，存档数据被清除，游戏回到初始状态：
![SavedData3](README.assets/SavedData3.png){width=600}
![SavedData1](README.assets/SavedData1.png){width=600}

## 2.5 剧

……

## 2.6 待补充

……

# 3. 组员及分工

