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
#include "core/entity/enemy/dog.h"
#include "core/entity/enemy/Life-up.h"
#include "core/entity/enemy/Not-attacked.h"


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
    const static int size = 140;
public:
    static void move(LevelScene *levelScene, Enemy *enemy, std::pair<size_t, size_t> currentPos,
                     std::pair<size_t, size_t> targetPos);
    
    static void releaseSkill(LevelScene *levelScene, Enemy *enemy);
    
    static void dead(LevelScene *levelScene, Enemy *enemy);
};

#endif //TOWERDEFENCE_ENEMYANIMATION_H
