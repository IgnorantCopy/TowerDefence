#ifndef DECELERATE_MAGICIAN_P_H
#define DECELERATE_MAGICIAN_P_H

#include "cocos2d.h"

class decelerate_magician_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(decelerate_magician_p);
};

#endif //DECELERATE_MAGICIAN_P_H
