#ifndef __ILLUSTRATION_H__
#define __ILLUSTRATION_H__

#include "cocos2d.h"

class illustration: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();



    CREATE_FUNC(illustration);

}

#endif  // __ILLUSTRATION_H__
