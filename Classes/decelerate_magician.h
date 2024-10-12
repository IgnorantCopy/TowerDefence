#ifndef DECELERATE_MAGICIAN_H
#define DECELERATE_MAGICIAN_H

#include "cocos2d.h"

class decelerate_magician: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(decelerate_magician);
};

#endif //DECELERATE_MAGICIAN_H
