#ifndef SPECIAL_MAGICIAN_P_H
#define SPECIAL_MAGICIAN_P_H

#include "cocos2d.h"

class special_magician_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(special_magician_p);
};

#endif //SPECIAL_MAGICIAN_P_H
