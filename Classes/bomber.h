#ifndef BOMBER_H
#define BOMBER_H

#include "cocos2d.h"

class bomber: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(bomber);
};

#endif //BOMBER_H
