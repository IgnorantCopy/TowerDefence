#ifndef PLOT_3_H
#define PLOT_3_H

#include "cocos2d.h"
#include "plot.h"

class plot_3 : public plot {
  public:
    static cocos2d::Scene *createScene();

    bool init() override;

    CREATE_FUNC(plot_3);
};

#endif // PLOT_3_H
