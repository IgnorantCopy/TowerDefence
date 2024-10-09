#ifndef TOWERDEFENCE_LEVELSCENE_H
#define TOWERDEFENCE_LEVELSCENE_H

#include "cocos2d.h"
#include "core/map.h"
#include "ui/CocosGUI.h"
using towerdefence::core::Map;
using towerdefence::core::Grid;

class LevelScene : public cocos2d::Scene
{
private:
    // the map of each level
    Map* map;
    Grid::Type type[7][12] = { Grid::Type::BlockPath };
    size_t height = 7, width = 12;

    // the sprite on the map
    std::vector<cocos2d::Sprite> enemy;
    std::vector<cocos2d::Sprite> tower;

    // select tower
    bool isSelcted = false;
    cocos2d::Sprite* selectedTower = nullptr;

public:
    // move the entity

    ~LevelScene();

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(LevelScene);
};

#endif //TOWERDEFENCE_LEVELSCENE_H
