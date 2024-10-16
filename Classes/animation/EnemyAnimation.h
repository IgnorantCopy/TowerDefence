//
// Created by Ignorant on 2024/10/12.
//

#ifndef TOWERDEFENCE_ENEMYANIMATION_H
#define TOWERDEFENCE_ENEMYANIMATION_H

#include "cocos2d.h"
#include "LevelScene.h"
#include "core/entity/entity.h"


using towerdefence::core::id::Id;
using towerdefence::core::Enemy;
using towerdefence::core::EnemyType;
using towerdefence::core::Grid;


class EnemyAnimation {
    const static int size = 140;
    constexpr const static float epsilon = 1e-6;
    static size_t transportCounter;
    static size_t attackDownCounter;
    static size_t notAttackedCounter;
public:
    static void move(LevelScene *levelScene, Enemy *enemy, std::pair<size_t, size_t> currentPos,
                     std::pair<size_t, size_t> targetPos);
    
    static void transport(LevelScene *levelScene, Enemy *enemy, std::pair<size_t, size_t> currentPos,
                          std::pair<size_t, size_t> targetPos);
    
    static void releaseSkill(LevelScene *levelScene, Enemy *enemy, float duration);
    
    static void dead(LevelScene *levelScene, Enemy *enemy);
};

#endif //TOWERDEFENCE_ENEMYANIMATION_H
