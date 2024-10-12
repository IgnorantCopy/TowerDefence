#ifndef AGGRESSIVE_MAGICIAN_P_H
#define AGGRESSIVE_MAGICIAN_P_H

#include "cocos2d.h"

class aggressive_magician_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(aggressive_magician_p);
};

#endif //AGGRESSIVE_MAGICIAN_P_H
