#ifndef ARCHER_BASE_H
#define ARCHER_BASE_H

#include "cocos2d.h"

class archer_base: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(archer_base);
};

#endif //ARCHER_BASE_H
