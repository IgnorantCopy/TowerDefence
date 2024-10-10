#ifndef TOWERDEFENCE_LEVEL1SCENE_H
#define TOWERDEFENCE_LEVEL1SCENE_H

#include "LevelScene.h"

class Level1Scene : public LevelScene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Level1Scene);
    
    void onMouseDown(cocos2d::Event *event);
    
    void onMouseUp(cocos2d::Event *event);
    
    void onMouseMove(cocos2d::Event *event);
};

#endif //TOWERDEFENCE_LEVEL1SCENE_H
