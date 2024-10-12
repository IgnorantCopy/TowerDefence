#ifndef MAGICIAN_BASE_H
#define MAGICIAN_BASE_H

#include "cocos2d.h"

class magician_base: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(magician_base);
};

#endif //MAGICIAN_BASE_H
