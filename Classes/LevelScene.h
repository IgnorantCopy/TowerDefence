#ifndef TOWERDEFENCE_LEVELSCENE_H
#define TOWERDEFENCE_LEVELSCENE_H

#include "animation/TowerAnimation.h"
#include "cocos2d.h"
#include "cocostudio/SimpleAudioEngine.h"
#include "core/entity/entity.h"
#include "core/entity/route.h"
#include "core/id.h"
#include "core/map.h"
#include "core/timer.h"
#include "ui/CocosGUI.h"

using towerdefence::core::EnemyType;
using towerdefence::core::Grid;
using towerdefence::core::Map;
using towerdefence::core::id::Id;
using towerdefence::core::route::Route;
using ssize = std::make_signed_t<size_t>;
using towerdefence::core::Enemy;
using towerdefence::core::EnemyFactory;
using towerdefence::core::EnemyFactoryBase;
using towerdefence::core::Tower;

class Bullet;

class LevelScene : public cocos2d::Scene {
  protected:
    int Level = 0;
    Grid::Type type[7][12] = {Grid::Type::BlockPath};
    std::vector<std::vector<size_t>> gridType;
    std::vector<Grid::Type> gridTypes = {
        Grid::Type::BlockPath,  Grid::Type::BlockIn,
        Grid::Type::BlockOut,   Grid::Type::BlockTransport,
        Grid::Type::BlockTower, Grid::Type::None};
    size_t width = 12, height = 7;
    // the size of each grid
    float SIZE = 140.0;
    // the images of each grid
    std::vector<std::string> images = {
        "images/block_low.png", "images/in.png", "images/out.png",
        "images/block_transport.png", "images/block_high.png"};
    // the button of each grid
    cocos2d::ui::Button *grid[7][12] = {nullptr};

    // the route of enemies
    enum DirType { U, D, L, R };
    std::vector<std::pair<ssize, ssize>> Dir = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::vector<Route> routes;

    // the FrameTime in 1s
    const uint32_t FrameTime = 30;
    // create enemies
    size_t enemyNumber = 0;
    std::vector<float> enemyCreateTime;
    std::vector<std::pair<size_t, size_t>> enemyStartPos;
    std::vector<DirType> enemyFirstDir;
    std::vector<std::vector<std::pair<size_t, size_t>>> enemyCreateType;
    std::vector<EnemyType> enemyType = {
        EnemyType::Worm,    EnemyType::Dog,         EnemyType::Soldier,
        EnemyType::Warlock, EnemyType::Destroyer,   EnemyType::Tank,
        EnemyType::Crab,    EnemyType::SpeedUp,     EnemyType::AttackDown,
        EnemyType::LifeUp,  EnemyType::NotAttacked, EnemyType::Boss1,
        EnemyType::Boss2};
    std::vector<float> enemyScale = {0.4,  0.2, 0.4,  0.4,  0.25, 0.4,
                                     0.25, 0.4, 0.25, 0.25, 0.35, 0.5};

    void createEnemy();

    // Show enemies
    std::vector<std::vector<cocos2d::Sprite *>> enemySprites;
    std::vector<std::vector<std::unique_ptr<EnemyFactoryBase>>> enemyFactories;
    std::vector<std::vector<std::pair<size_t, size_t>>> enemyPos;

    // update the ui
    void update();

    void updateParticles();

    void updateBullets();

    void updateMoneyLabel();

    void decreaseLife();

    void updateSelectorEnabled();

    void updateUpgradeItemEnabled();

    void updateUpgradeButtonEnabled();

    // select tower
    int isSelecting = 0;
    cocos2d::MenuItemLabel *backItem = nullptr;
    cocos2d::ui::Button *archerBaseSelector = nullptr;
    cocos2d::ui::Button *magicianBaseSelector = nullptr;
    cocos2d::ui::Button *helperBaseSelector = nullptr;
    cocos2d::Sprite *selectedTower = nullptr;
    cocos2d::Label *moneyLabel = nullptr;
    cocos2d::Label *lifeLabel = nullptr;

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
    bool isUpgrade = false;
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
    uint32_t upgradeTowerCost = 0;
    uint32_t upgradeTower1Cost = 0;
    uint32_t upgradeTower2Cost = 0;
    uint32_t upgradeTower3Cost = 0;
    cocos2d::MenuItemLabel *upgradeItem1 = nullptr;
    cocos2d::MenuItemLabel *upgradeItem2 = nullptr;
    cocos2d::MenuItemLabel *upgradeItem3 = nullptr;
    cocos2d::MenuItemLabel *cancelUpgradeItem = nullptr;

    void addBullet(Tower *tower, Enemy *enemy);

    void upgradeTower();

    void showUpgradeMenu();

    void hideUpgradeMenu();

    void executeSkill();

    void cancelSelect();

    void putTower(float x, float y);

    void showTowerInfo(float x, float y);

    void hideTowerInfo(float x, float y);

    bool gameContinuing = true;
    bool Win = false;
    void gameOver(bool isWin);

  public:
    // the map of each level
    Map *map = nullptr;
    // towers
    std::vector<std::pair<Id, cocos2d::Sprite *>> towers;
    // enemies
    std::vector<std::pair<Id, cocos2d::Sprite *>> enemies;
    // bullets
    std::vector<Bullet *> bullets;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref *pSender);

    // create the map
    void createMap(int level);

    void onMouseDown(cocos2d::Event *event);

    void onMouseUp(cocos2d::Event *event);

    void onMouseMove(cocos2d::Event *event);

    cocos2d::Sprite *getTower(Id id);

    cocos2d::Sprite *getEnemy(Id id);

    void deleteTower(bool isReturn = true);

    virtual bool init(int level);

    ~LevelScene() override;
};

#endif // TOWERDEFENCE_LEVELSCENE_H
