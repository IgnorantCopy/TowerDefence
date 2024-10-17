#ifndef TOWERDEFENCE_LEVEL2SCENE_H
#define TOWERDEFENCE_LEVEL2SCENE_H

#include "LevelScene.h"

class Level2Scene : public LevelScene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Level2Scene);
};

#endif //TOWERDEFENCE_LEVEL2SCENE_H
