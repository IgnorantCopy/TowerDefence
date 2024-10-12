#ifndef ARCHER_H
#define ARCHER_H

#include "cocos2d.h"

class archer: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(archer);
};
#endif //ARCHER_H
