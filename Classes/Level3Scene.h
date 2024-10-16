#ifndef TOWERDEFENCE_LEVEL3SCENE_H
#define TOWERDEFENCE_LEVEL3SCENE_H

#include "LevelScene.h"

class Level3Scene : public LevelScene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Level3Scene);
};

#endif //TOWERDEFENCE_LEVEL3SCENE_H
