#ifndef TOWERDEFENCE_LEVEL3SCENE_H
#define TOWERDEFENCE_LEVEL3SCENE_H

#include "cocos2d.h"
#include "core/map.h"

class Level3Scene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Level3Scene);
};

#endif //TOWERDEFENCE_LEVEL3SCENE_H
