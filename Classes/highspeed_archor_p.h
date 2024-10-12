#ifndef HIGHSPEED_ARCHOR_P_H
#define HIGHSPEED_ARCHOR_P_H

#include "cocos2d.h"

class highspeed_archor_p : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(highspeed_archor_p);
};

#endif //HIGHSPEED_ARCHOR_P_H
