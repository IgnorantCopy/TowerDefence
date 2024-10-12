#ifndef ARCHER_P_H
#define ARCHER_P_H

#include "cocos2d.h"

class archer_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(archer_p);
};
#endif //ARCHER_P_H
