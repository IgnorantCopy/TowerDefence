#ifndef TOWERDEFENCE_LEVELSCENE_H
#define TOWERDEFENCE_LEVELSCENE_H

#include "cocos2d.h"
#include "core/map.h"
#include "ui/CocosGUI.h"
using towerdefence::core::Grid;
using towerdefence::core::Map;
using namespace towerdefence::core::id;

class LevelScene : public cocos2d::Scene {

protected:
    // the map of each level
    Map *map = nullptr;
    Grid::Type type[7][12] = { Grid::Type::BlockPath };
    size_t width = 12, height = 7;
    // the size of each grid
    float SIZE = 140.0;
    // the images of each grid
    std::vector<std::string> images = { "images/block_low.png", "images/in.png", "images/out.png",
                                        "images/block_transport.png", "images/block_high.png" };
    // the button of each grid
    cocos2d::ui::Button* grid[7][12]={ nullptr };
    
    // enemies
    std::vector<std::pair<Id, cocos2d::Sprite*>> enemies;
    // towers
    std::vector<std::pair<Id, cocos2d::Sprite*>> towers;

    // select tower
    int isSelecting = 0;
    cocos2d::Sprite *selectedTower = nullptr;
    cocos2d::Label *moneyLabel = nullptr;
    
    // tower info
    Id selectedTowerId;
    bool isShowingTowerInfo = false;
    cocos2d::MenuItemImage *deleteItem = nullptr;
    cocos2d::MenuItemImage *upgradeItem = nullptr;
    cocos2d::MenuItemImage *towerInfoItem = nullptr;
    cocos2d::MenuItemImage *skillItem = nullptr;
    
    cocos2d::Sprite* getTower(Id id);
    cocos2d::Sprite* getEnemy(Id id);
    void deleteTower();
    void upgradeTower();
    void showTowerInfo();
    void executeSkill();
    
    void cancelSelect();
    void putTower(float x, float y);
    void showTowerInfo(float x, float y);
    void hideTowerInfo(float x, float y);
    
public:
    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // create the map
    void createMap(int level);
    
    void onMouseDown(cocos2d::Event *event);
    
    void onMouseUp(cocos2d::Event *event);
    
    void onMouseMove(cocos2d::Event *event);
};

#endif //TOWERDEFENCE_LEVELSCENE_H
