#ifndef PLOT_1_H
#define PLOT_1_H

#include "cocos2d.h"
#include "plot.h"

class plot_1 : public plot {
  public:
    static cocos2d::Scene *createScene();

    bool init() override;

    CREATE_FUNC(plot_1);
};

#endif // PLOT_1_H