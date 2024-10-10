#ifndef TOWERDEFENCE_LEVELSCENE_H
#define TOWERDEFENCE_LEVELSCENE_H

#include "cocos2d.h"
#include "core/map.h"
#include "core/id.h"
#include "core/entity/entity.h"
#include "ui/CocosGUI.h"
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
#include "core/entity/tower/highspeed_archor.h"
#include "core/entity/tower/highspeed_archor_plus.h"
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

    // the tower on the map
    std::vector<std::pair<Id,cocos2d::Sprite*>> towers;
    // the enemy on the map
    std::vector<std::pair<Id,cocos2d::Sprite*>> enemies;

    // select tower
    bool isSelecting = false;
    cocos2d::Sprite *selectedTower = nullptr;

public:
    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // create the map
    void createMap(int level);
    // get the grid at (x,y)
    Grid& getGrid(size_t x, size_t y);
    // find the tower by id
    cocos2d::Sprite* findTower(Id id);
    // find the enemy by id
    cocos2d::Sprite* findEnemy(Id id);

    ~LevelScene() override;
};

#endif //TOWERDEFENCE_LEVELSCENE_H
