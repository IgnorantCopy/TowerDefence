#ifndef HELPER_BASE_H
#define HELPER_BASE_H

#include "cocos2d.h"

class helper_base: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(helper_base);
};

#endif //HELPER_BASE_H
