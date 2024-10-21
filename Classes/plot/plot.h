//
// Created by Ignorant on 2024/10/21.
//

#ifndef TOWERDEFENCE_PLOT_H
#define TOWERDEFENCE_PLOT_H

#include "cocos2d.h"

class plot : public cocos2d::Scene {
  protected:
    cocos2d::Sprite *backgroundOver = nullptr;
    cocos2d::Sprite *secret = nullptr;
    bool isShowingSecret = false;

  public:
    virtual bool init();

    CREATE_FUNC(plot);

    void onMouseDown(cocos2d::Event *event);

    void onMouseUp(cocos2d::Event *event);

    void showSecret();

    void hideSecret();
};

#endif // TOWERDEFENCE_PLOT_H
