#ifndef DIFFUSIVE_MAGICIAN_H
#define DIFFUSIVE_MAGICIAN_H

#include "cocos2d.h"

class diffusive_magician: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(diffusive_magician);
};

#endif //DIFFUSIVE_MAGICIAN_H
