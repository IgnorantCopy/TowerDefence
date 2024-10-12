#ifndef AGGRESSIVE_MAGICIAN_H
#define AGGRESSIVE_MAGICIAN_H

#include "cocos2d.h"

class aggressive_magician: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(aggressive_magician);
};

#endif //AGGRESSIVE_MAGICIAN_H
