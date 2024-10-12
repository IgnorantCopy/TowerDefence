#ifndef SPECIAL_MAGICIAN_H
#define SPECIAL_MAGICIAN_H

#include "cocos2d.h"

class special_magician: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(special_magician);
};

#endif //SPECIAL_MAGICIAN_H
