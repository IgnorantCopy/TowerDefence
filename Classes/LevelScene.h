#ifndef TOWERDEFENCE_LEVELSCENE_H
#define TOWERDEFENCE_LEVELSCENE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "core/map.h"
#include "core/timer.h"
#include "core/id.h"
#include "core/entity/entity.h"
#include "core/entity/tower/aggressive_magician.h"
#include "core/entity/tower/aggressive_magician_plus.h"
#include "core/entity/tower/archer.h"
#include "core/entity/tower/archer_base.h"
#include "core/entity/tower/archer_plus.h"
#include "core/entity/tower/bomber.h"
#include "core/entity/tower/bomber_plus.h"
#include "core/entity/tower/core_magician.h"
#include "core/entity/tower/core_magician_plus.h"
#include "core/entity/tower/decelerate_magician.h"
#include "core/entity/tower/decelerate_magician_plus.h"
#include "core/entity/tower/diffusive_magician.h"
#include "core/entity/tower/diffusive_magician_plus.h"
#include "core/entity/tower/helper_base.h"
#include "core/entity/tower/highspeed_archer.h"
#include "core/entity/tower/highspeed_archer_plus.h"
#include "core/entity/tower/magician_base.h"
#include "core/entity/tower/special_magician.h"
#include "core/entity/tower/special_magician_plus.h"
#include "core/entity/tower/weaken_magician.h"
#include "core/entity/tower/weaken_magician_plus.h"
#include "core/entity/enemy/Attack-down.h"
#include "core/entity/enemy/Boss-1.h"
#include "core/entity/enemy/Boss-2.h"
#include "core/entity/enemy/Crab.h"
#include "core/entity/enemy/Destroyer.h"
#include "core/entity/enemy/dog.h"
#include "core/entity/enemy/Life-up.h"
#include "core/entity/enemy/Not-attacked.h"
#include "core/entity/enemy/Soldier.h"
#include "core/entity/enemy/Speed-up.h"
#include "core/entity/enemy/Tank.h"
#include "core/entity/enemy/Warlock.h"
#include "core/entity/enemy/worm.h"

using namespace towerdefence::core;
using towerdefence::core::id::Id;

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

    // update the ui
    void update();

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
