#ifndef TOWERDEFENCE_LEVELSCENE_H
#define TOWERDEFENCE_LEVELSCENE_H

#include "cocos2d.h"
#include "core/map.h"
#include "ui/CocosGUI.h"
#include "animation/TowerAnimation.h"
using towerdefence::core::Grid;
using towerdefence::core::Map;
using towerdefence::core::id::Id;

class Bullet;
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
    cocos2d::ui::Button *archerBaseSelector = nullptr;
    cocos2d::ui::Button *magicianBaseSelector = nullptr;
    cocos2d::ui::Button *helperBaseSelector = nullptr;
    cocos2d::Sprite *selectedTower = nullptr;
    cocos2d::Label *moneyLabel = nullptr;
    
    // tower info
    const int archerBaseCost = 7;
    const int highspeedArcherCost = 10;
    const int highspeedArcherProCost = 15;
    const int bomberCost = 16;
    const int bomberProCost = 20;
    const int archerCost = 18;
    const int archerProCost = 24;
    const int magicianBaseCost = 9;
    const int coreMagicianCost = 12;
    const int coreMagicianProCost = 16;
    const int diffusiveMagicianCost = 15;
    const int diffusiveMagicianProCost = 21;
    const int specialMagicianCost = 14;
    const int specialMagicianProCost = 25;
    const int helperBaseCost = 5;
    const int decelerateMagicianCost = 9;
    const int decelerateMagicianProCost = 16;
    const int weakenMagicianCost = 13;
    const int weakenMagicianProCost = 18;
    const int aggressiveMagicianCost = 8;
    const int aggressiveMagicianProCost = 14;
    Id selectedTowerId;
    bool isShowingTowerInfo = false;
    cocos2d::Menu *upgradeMenu = nullptr;
    cocos2d::ui::Button *deleteButton = nullptr;
    cocos2d::ui::Button *upgradeButton = nullptr;
    cocos2d::ui::Button *towerInfoButton = nullptr;
    cocos2d::ui::Button *skillButton = nullptr;
    cocos2d::Sprite *upgradeBackground1 = nullptr;
    cocos2d::Sprite *upgradeBackground2 = nullptr;
    cocos2d::Sprite *upgradeBackground3 = nullptr;
    cocos2d::Sprite *upgradeTower1 = nullptr;
    cocos2d::Sprite *upgradeTower2 = nullptr;
    cocos2d::Sprite *upgradeTower3 = nullptr;
    cocos2d::MenuItemLabel *upgradeItem1 = nullptr;
    cocos2d::MenuItemLabel *upgradeItem2 = nullptr;
    cocos2d::MenuItemLabel *upgradeItem3 = nullptr;
    cocos2d::MenuItemLabel *cancelUpgradeItem = nullptr;
    
    void updateSelectorEnabled();
    
    void deleteTower(bool isReturn = true);
    void upgradeTower();
    void showUpgradeMenu();
    void hideUpgradeMenu();
    void showTowerInfo();
    void executeSkill();
    
    void cancelSelect();
    void putTower(float x, float y);
    void showTowerInfo(float x, float y);
    void hideTowerInfo(float x, float y);
    
public:
    // bullets
    std::vector<Bullet*> bullets;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // create the map
    void createMap(int level);
    
    void onMouseDown(cocos2d::Event *event);
    void onMouseUp(cocos2d::Event *event);
    void onMouseMove(cocos2d::Event *event);
    
    cocos2d::Sprite* getTower(Id id);
    cocos2d::Sprite* getEnemy(Id id);
    
    void addBullet(Bullet *bullet);
};

#endif //TOWERDEFENCE_LEVELSCENE_H
