#ifndef PLOT_1_H
#define PLOT_1_H

#include "cocos2d.h"

class plot_1: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(plot_1);
};

#endif //PLOT_1_H