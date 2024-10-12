//
// Created by Ignorant on 2024/10/12.
//

#ifndef TOWERDEFENCE_TOWERANIMATION_H
#define TOWERDEFENCE_TOWERANIMATION_H

#include "cocos2d.h"
#include "LevelScene.h"
#include "core/entity/entity.h"
#include <math.h>


USING_NS_CC;
using towerdefence::core::id::Id;
using towerdefence::core::Enemy;
using towerdefence::core::Tower;
using towerdefence::core::TowerType;

class Bullet {
    Sprite *bullet;
    Tower *tower;
    Enemy *enemy;
    LevelScene *levelScene;
    float angle;
    
public:
    Bullet(LevelScene* levelScene, Tower* tower, Enemy* enemy): levelScene(levelScene), tower(tower), enemy(enemy) {
        switch (tower->status().tower_type_) {
            case TowerType::ArcherBase:
                this->bullet = Sprite::create("images/bullet/arrows/arrow_basic.png");
                break;
            case TowerType::Archer:
            case TowerType::ArcherPlus:
                this->bullet = Sprite::create("images/bullet/arrows/archer_arrow.png");
                break;
            case TowerType::HighspeedArcher:
            case TowerType::HighspeedArcherPlus:
                this->bullet = Sprite::create("images/bullet/arrows/highspeed_arrow.png");
                break;
            case TowerType::MagicianBase:
            case TowerType::DecelerateMagician:
            case TowerType::DecelerateMagicianPlus:
                this->bullet = Sprite::create("images/bullet/blueBullet/blueBullet00.png");
                break;
            case TowerType::CoreMagician:
            case TowerType::CoreMagicianPlus:
            case TowerType::WeakenMagician:
            case TowerType::WeakenMagicianPlus:
                this->bullet = Sprite::create("images/bullet/purpleBullet/purpleBullet00.png");
                break;
            case TowerType::DiffusiveMagician:
            case TowerType::DiffusiveMagicianPlus:
            case TowerType::HelperBase:
                this->bullet = Sprite::create("images/bullet/greenBullet/greenBullet00.png");
                break;
            case TowerType::SpecialMagician:
            case TowerType::SpecialMagicianPlus:
            case TowerType::AggressiveMagician:
            case TowerType::AggressiveMagicianPlus:
                this->bullet = Sprite::create("images/bullet/orangeBullet/orangeBullet00.png");
                break;
            default:
                this->bullet = Sprite::create("images/bullet/arrows/arrow_basic.png");
                break;
        }
        
    }
};

class TowerAnimation {
public:
    static void attack(LevelScene* levelScene, Tower* tower, Enemy* enemy) {
        Id towerId = tower->id;
        Id enemyId = enemy->id;
        Sprite *towerSprite = levelScene->getTower(towerId);
        float towerX = towerSprite->getPositionX();
        float towerY = towerSprite->getPositionY();
        Sprite *enemySprite = levelScene->getEnemy(enemyId);
        float enemyX = enemySprite->getPositionX();
        float enemyY = enemySprite->getPositionY();
        Sprite *bulletSprite;
        switch (tower->status().tower_type_) {
            case TowerType::ArcherBase:
                bulletSprite = Sprite::create("images/bullet/arrows/arrow_basic.png");
                break;
            case TowerType::Archer:
            case TowerType::ArcherPlus:
                bulletSprite = Sprite::create("images/bullet/arrows/archer_arrow.png");
                break;
            case TowerType::HighspeedArcher:
            case TowerType::HighspeedArcherPlus:
                bulletSprite = Sprite::create("images/bullet/arrows/highspeed_arrow.png");
                break;
            case TowerType::MagicianBase:
            case TowerType::DecelerateMagician:
            case TowerType::DecelerateMagicianPlus:
                bulletSprite = Sprite::create("images/bullet/blueBullet/blueBullet00.png");
                break;
            case TowerType::CoreMagician:
            case TowerType::CoreMagicianPlus:
            case TowerType::WeakenMagician:
            case TowerType::WeakenMagicianPlus:
                bulletSprite = Sprite::create("images/bullet/purpleBullet/purpleBullet00.png");
                break;
            case TowerType::DiffusiveMagician:
            case TowerType::DiffusiveMagicianPlus:
            case TowerType::HelperBase:
                bulletSprite = Sprite::create("images/bullet/greenBullet/greenBullet00.png");
                break;
            case TowerType::SpecialMagician:
            case TowerType::SpecialMagicianPlus:
            case TowerType::AggressiveMagician:
            case TowerType::AggressiveMagicianPlus:
                bulletSprite = Sprite::create("images/bullet/orangeBullet/orangeBullet00.png");
                break;
            default:
                bulletSprite = Sprite::create("images/bullet/arrows/arrow_basic.png");
                break;
        }
        bulletSprite->setPosition(towerX, towerY);
        float angle = 0.0f;
        if (enemyX == towerX) {
            if (enemyY < towerY) {
                angle = 90.0f;
            } else {
                angle = -90.0f;
            }
        } else {
            angle = (float)(atan((towerY - enemyY) / (enemyX - towerX)) * 180.0f / M_PI);
        }
        bulletSprite->setRotation(angle);
        levelScene->addChild(bulletSprite, 4);
    }
};

#endif //TOWERDEFENCE_TOWERANIMATION_H
