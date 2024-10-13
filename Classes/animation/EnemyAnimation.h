//
// Created by Ignorant on 2024/10/12.
//

#ifndef TOWERDEFENCE_ENEMYANIMATION_H
#define TOWERDEFENCE_ENEMYANIMATION_H

#include "cocos2d.h"
#include "LevelScene.h"
#include "core/entity/entity.h"
#include "core/entity/enemy/Attack-down.h"
#include "core/entity/enemy/Boss-1.h"
#include "core/entity/enemy/Boss-2.h"
#include "core/entity/enemy/Crab.h"
#include "core/entity/enemy/Destroyer.h"
#include "core/entity/enemy/Dog.h"
#include "core/entity/enemy/Life-up.h"
#include "core/entity/enemy/Not-attacked.h"


USING_NS_CC;
using towerdefence::core::id::Id;
using towerdefence::core::Enemy;
using towerdefence::core::EnemyType;
using towerdefence::core::AttackDown;
using towerdefence::core::Boss1;
using towerdefence::core::Boss2;
using towerdefence::core::Crab;
using towerdefence::core::Destroyer;
using towerdefence::core::Dog;
using towerdefence::core::LifeUp;
using towerdefence::core::NotAttacked;


class EnemyAnimation {
public:
    static void move(LevelScene *levelScene, Enemy *enemy) {
        std::string prefix = "images/enemies/";
        switch (enemy->status().enemy_type_) {
            case EnemyType::AttackDown:
                prefix += "attackDown/move/attackDown_move";
                break;
            case EnemyType::Boss1:
                prefix += "boss/stage1/move";
                break;
            case EnemyType::Boss2:
                prefix += "boss/stage2";
                break;
            case EnemyType::Crab:
                prefix += "crab/move/crab_move";
                break;
            case EnemyType::Destroyer:
                prefix += "destroyer/move/destroyer_move";
                break;
            case EnemyType::Dog:
                prefix += "dog/move/dog_move";
                break;
            case EnemyType::LifeUp:
                prefix += "lifeUp/move/lifeUp_move";
                break;
            case EnemyType::NotAttacked:
                prefix += "notAttacked/move/notAttacked_move";
                break;
            case EnemyType::Soldier:
                prefix += "soldier/move/soldier_move";
                break;
            case EnemyType::SpeedUp:
                prefix += "speedUp/move/speedUp_move";
                break;
            case EnemyType::Tank:
                prefix += "tank/move/tank_move";
                break;
            case EnemyType::Warlock:
                prefix += "warlock/move/warlock_move";
                break;
            case EnemyType::Worm:
                prefix += "worm/move/worm_move";
                break;
            default:
                break;
        }
        std::string movePath = std::format("{:02d}.png", enemy->status().current_frame_);
        Id id = enemy->id;
        auto enemySprite = levelScene->getEnemy(id);
        enemySprite->setTexture(prefix + movePath);
        
    }
};

#endif //TOWERDEFENCE_ENEMYANIMATION_H
