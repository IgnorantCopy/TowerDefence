#ifndef CORE_MAGICIAN_H
#define CORE_MAGICIAN_H

#include "cocos2d.h"

class core_magician: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(core_magician);
};

#endif //CORE_MAGICIAN_H
