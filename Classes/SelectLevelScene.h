#ifndef TOWERDEFENCE_SELECTLEVELSCENE_H
#define TOWERDEFENCE_SELECTLEVELSCENE_H

#include "cocos2d.h"

class SelectLevelScene : public cocos2d::Scene
{
    int level1Scene = 2;
    int level2Scene = 3;
    int level3Scene = -1; // TODO: save the file to update these values
    cocos2d::Sprite *level1Star = nullptr;
    cocos2d::Sprite *level2Star = nullptr;
    cocos2d::Sprite *level3Star = nullptr;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SelectLevelScene);
    
    void onMouseDown(cocos2d::Event *event);
    
};

#endif //TOWERDEFENCE_SELECTLEVELSCENE_H
