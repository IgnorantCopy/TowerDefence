#ifndef TOWERDEFENCE_LEVEL1SCENE_H
#define TOWERDEFENCE_LEVEL1SCENE_H

#include "LevelScene.h"

class Level1Scene : public LevelScene
{
public:
    static cocos2d::Scene *createScene();
    
    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Level1Scene);
};

#endif //TOWERDEFENCE_LEVEL1SCENE_H
