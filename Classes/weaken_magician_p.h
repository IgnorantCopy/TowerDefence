#ifndef WEAKEN_MAGICIAN_P_H
#define WEAKEN_MAGICIAN_P_H

#include "cocos2d.h"

class weaken_magician_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(weaken_magician_p);
};

#endif //WEAKEN_MAGICIAN_P_H
