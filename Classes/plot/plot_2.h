#ifndef PLOT_2_H
#define PLOT_2_H

#include "cocos2d.h"
#include "plot.h"

class plot_2 : public plot {
  public:
    static cocos2d::Scene *createScene();

    bool init() override;

    CREATE_FUNC(plot_2);
};

#endif // PLOT_2_H
