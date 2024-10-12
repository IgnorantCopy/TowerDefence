#ifndef DIFFUSIVE_MAGICIAN_P_H
#define DIFFUSIVE_MAGICIAN_P_H

#include "cocos2d.h"

class diffusive_magician_p: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(diffusive_magician_p);
};


#endif //DIFFUSIVE_MAGICIAN_P_H
