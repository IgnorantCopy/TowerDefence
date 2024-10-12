#ifndef CORE_MAGICIAN_P_H
#define CORE_MAGICIAN_P_H

#include "cocos2d.h"

class core_magician_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(core_magician_p);
};

#endif //CORE_MAGICIAN_P_H
