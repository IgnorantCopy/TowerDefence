#ifndef PLOT_4_H
#define PLOT_4_H

#include "cocos2d.h"
#include "plot.h"

class plot_4 : public plot {
  public:
    static cocos2d::Scene *createScene();

    bool init() override;

    CREATE_FUNC(plot_4);
};

#endif // PLOT_4_H
