#ifndef TOWERDEFENCE_LEVEL1SCENE_H
#define TOWERDEFENCE_LEVEL1SCENE_H

#include "cocos2d.h"
//#include "core/map.h"

class Level1Scene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1Scene);
};

#endif //TOWERDEFENCE_LEVEL1SCENE_H
