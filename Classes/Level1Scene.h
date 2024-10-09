#ifndef TOWERDEFENCE_LEVEL1SCENE_H
#define TOWERDEFENCE_LEVEL1SCENE_H

#include "cocos2d.h"
#include "core/map.h"

class Level1Scene : public cocos2d::Scene
{
    bool isSelecting = false;
    cocos2d::Sprite *selectedTower = nullptr;
    cocos2d::Label *moneyLabel = nullptr;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1Scene);
    
    void onMouseDown(cocos2d::Event *event);
    
    void onMouseUp(cocos2d::Event *event);
    
    void onMouseMove(cocos2d::Event *event);
};

#endif //TOWERDEFENCE_LEVEL1SCENE_H
