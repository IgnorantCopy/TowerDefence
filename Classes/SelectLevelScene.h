#ifndef TOWERDEFENCE_SELECTLEVELSCENE_H
#define TOWERDEFENCE_SELECTLEVELSCENE_H

#include "cocos2d.h"

class SelectLevelScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SelectLevelScene);
};

#endif //TOWERDEFENCE_SELECTLEVELSCENE_H
