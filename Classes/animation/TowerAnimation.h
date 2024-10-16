//
// Created by Ignorant on 2024/10/12.
//

#ifndef TOWERDEFENCE_TOWERANIMATION_H
#define TOWERDEFENCE_TOWERANIMATION_H

#include "LevelScene.h"
#include "cocos2d.h"
#include "core/entity/entity.h"
#include <cmath>

using towerdefence::core::Enemy;
using towerdefence::core::Tower;
using towerdefence::core::TowerType;
using towerdefence::core::id::Id;

class LevelScene;

class Bullet {
protected:
    cocos2d::Sprite *bullet;
    Tower *tower;
    Enemy *enemy;
    LevelScene *levelScene;
    float dx;
    float dy;
    float angle;
    const float speed = 10;
    bool isScaling = false;
    bool isExploding = false;

public:
    Bullet(LevelScene *levelScene, Tower *tower, Enemy *enemy);

    cocos2d::Sprite *getBullet() const { return bullet; }

    virtual void move();

    virtual void explosion();

    void updateAngle();

    bool isTouch();
};

class MagicBullet : public Bullet {
    const int totalFrames = 60;
    int currentFrame = 0;
    std::string color;

public:
    MagicBullet(LevelScene *levelScene, Tower *tower, Enemy *enemy);

    void move() override;

    void explosion() override;
};

class TowerAnimation {
    constexpr const static float epsilon = 1e-6;
    constexpr const static float size = 140.0f;
    static size_t removeCounter;

public:
    static void releaseSkill(LevelScene *levelScene, Tower *tower,
                             float duration);
};

#endif // TOWERDEFENCE_TOWERANIMATION_H
