#ifndef HIGHSPEED_ARCHOR_H
#define HIGHSPEED_ARCHOR_H

#include "cocos2d.h"

class highspeed_archor: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(highspeed_archor);
};

#endif //HIGHSPEED_ARCHOR_H
