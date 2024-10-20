//
// Created by Ignorant on 2024/10/12.
//

#ifndef TOWERDEFENCE_ENEMYANIMATION_H
#define TOWERDEFENCE_ENEMYANIMATION_H

#include "LevelScene.h"
#include "cocos2d.h"
#include "core/entity/entity.h"

using towerdefence::core::Enemy;
using towerdefence::core::EnemyType;
using towerdefence::core::Grid;
using towerdefence::core::id::Id;

class EnemyAnimation {
    const static int size = 140;
    constexpr const static float epsilon = 1e-6;
    static size_t notAttackedCounter;
    static std::unique_ptr<EnemyFactoryBase> boss;

  public:
    static void move(LevelScene *levelScene, Enemy *enemy,
                     std::pair<size_t, size_t> currentPos,
                     std::pair<size_t, size_t> targetPos);

    static void transport(LevelScene *levelScene, Enemy *enemy,
                          std::pair<size_t, size_t> currentPos,
                          std::pair<size_t, size_t> targetPos);

    static void releaseSkill(LevelScene *levelScene, Enemy *enemy,
                             float duration);

    static void dead(LevelScene *levelScene, Enemy *enemy);
};

#endif // TOWERDEFENCE_ENEMYANIMATION_H
