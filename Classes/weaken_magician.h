#ifndef WEAKEN_MAGICIAN_H
#define WEAKEN_MAGICIAN_H

#include "cocos2d.h"

class weaken_magician: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(weaken_magician);
};

#endif //WEAKEN_MAGICIAN_H
