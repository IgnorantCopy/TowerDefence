#ifndef BOMBER_P_H
#define BOMBER_P_H

#include "cocos2d.h"

class bomber_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(bomber_p);
};

#endif //BOMBER_P_H
