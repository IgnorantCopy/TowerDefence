//
// Created by Ignorant on 2024/10/12.
//

#ifndef TOWERDEFENCE_TOWERANIMATION_H
#define TOWERDEFENCE_TOWERANIMATION_H

#include "cocos2d.h"
#include "LevelScene.h"
#include "core/entity/entity.h"
#include <cmath>


using towerdefence::core::id::Id;
using towerdefence::core::Enemy;
using towerdefence::core::Tower;
using towerdefence::core::TowerType;


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

public:
    Bullet(LevelScene *levelScene, Tower *tower, Enemy *enemy);
    
    virtual ~Bullet() = default;
    
    cocos2d::Sprite *getBullet() const { return bullet; }
    
    virtual void move();
    
    void updateAngle();
    
    bool isTouch();
};


class MagicBullet : public Bullet {
    const int totalFrames = 60;
    int currentFrame = 0;
    std::string color;

public:
    MagicBullet(LevelScene *levelScene, Tower *tower, Enemy *enemy, std::string color);
    
    ~MagicBullet() override { this->explosion(); }
    
    void move() override;
    
    void explosion();
};

#endif //TOWERDEFENCE_TOWERANIMATION_H
