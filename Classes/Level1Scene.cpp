#include "Level1Scene.h"
#include "SelectLevelScene.h"
#include "cocostudio/SimpleAudioEngine.h"
#include "core/entity/tower/archer.h"
#include "core/entity/tower/highspeed_archer.h"
#include "core/entity/tower/bomber.h"
#include "core/entity/tower/core_magician.h"
#include "core/entity/tower/diffusive_magician.h"
#include "core/entity/tower/special_magician.h"
#include "core/entity/tower/decelerate_magician.h"
#include "core/entity/tower/weaken_magician.h"
#include "core/entity/tower/aggressive_magician.h"

USING_NS_CC;
using towerdefence::core::Archer;
using towerdefence::core::HighspeedArcher;
using towerdefence::core::Bomber;
using towerdefence::core::CoreMagician;
using towerdefence::core::DiffusiveMagician;
using towerdefence::core::SpecialMagician;
using towerdefence::core::DecelerateMagician;
using towerdefence::core::WeakenMagician;
using towerdefence::core::AggressiveMagician;
using towerdefence::core::TowerType;
using towerdefence::core::TowerFactory;
using towerdefence::core::TowerFactoryBase;

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level1Scene.cpp\n");
}

bool Level1Scene::init()
{
    if ( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // add bgm
    auto player = CocosDenshion::SimpleAudioEngine::getInstance();
    player->playBackgroundMusic("audio/level1_bgm.MP3", true);

    auto background = Sprite::create("images/level1_background.png",Rect(0,0,2500,1500));
    if(background == nullptr) {
        problemLoading("'images/level1_background.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }
    
    auto frameBase = Sprite::create("images/frame_base.png");
    if(frameBase == nullptr) {
        problemLoading("'images/frame_base.png'");
    } else {
        frameBase->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 1500 - visibleSize.height));
        this->addChild(frameBase, 1);
    }
    
    float gap = 300;
    this->selectedTower = Sprite::create("images/towers/archer_base_onblock.png");
    this->selectedTower->setVisible(false);
    this->selectedTower->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(this->selectedTower, 5);
    this->archerBaseSelector = ui::Button::create(
            "images/towers/archer_base.png",
            "images/towers/archer_base.png",
            "images/towers/archer_base_inactive.png"
    );
    this->archerBaseSelector->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                this->isSelecting = 1;
                this->selectedTower->setTexture("images/towers/archer_base_onblock.png");
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });
    this->archerBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 - 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    this->addChild(this->archerBaseSelector, 4);
    
    this->magicianBaseSelector = ui::Button::create(
            "images/towers/magician_base.png",
            "images/towers/magician_base.png",
            "images/towers/magician_base_inactive.png"
    );
    this->magicianBaseSelector->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                this->isSelecting = 2;
                this->selectedTower->setTexture("images/towers/magician_base_onblock.png");
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });
    this->magicianBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                           origin.y + 1680 - visibleSize.height));
    this->addChild(this->magicianBaseSelector, 4);
    
    this->helperBaseSelector = ui::Button::create(
            "images/towers/helper_base.png",
            "images/towers/helper_base.png",
            "images/towers/helper_base_inactive.png"
    );
    this->helperBaseSelector->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                this->isSelecting = 3;
                this->selectedTower->setTexture("images/towers/helper_base_onblock.png");
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });
    this->helperBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 + 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    this->addChild(this->helperBaseSelector, 4);
    
    // tower info
    this->deleteButton = ui::Button::create(
            "images/delete.png",
            "images/delete.png",
            "images/delete.png"
    );
    this->deleteButton->setVisible(false);
    this->deleteButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->deleteTower();
                this->hideTowerInfo(0, 0);
                break;
            default:
                break;
        }
    });
    this->upgradeButton = ui::Button::create(
            "images/upgrade.png",
            "images/upgrade.png",
            "images/upgrade_inactive.png"
    );
    this->upgradeButton->setVisible(false);
    this->upgradeButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->upgradeTower();
                this->hideTowerInfo(0, 0);
                break;
            default:
                break;
        }
    });
    this->towerInfoButton = ui::Button::create(
            "images/info.png",
            "images/info.png",
            "images/info.png"
    );
    this->towerInfoButton->setVisible(false);
    this->towerInfoButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->showTowerInfo();
                break;
            default:
                break;
        }
    });
    this->skillButton = ui::Button::create(
            "images/towers/skill_icon/archer_base.png",
            "images/towers/skill_icon/archer_base.png",
            "images/towers/skill_icon/archer_base_inactive.png"
    );
    this->skillButton->setVisible(false);
    this->skillButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->executeSkill();
                break;
            default:
                break;
        }
    });
    this->addChild(this->deleteButton, 4);
    this->addChild(this->upgradeButton, 4);
    this->addChild(this->towerInfoButton, 4);
    this->addChild(this->skillButton, 4);
    
    // the back button to go back to the SelectLevel scene
    auto Back=Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem=MenuItemLabel::create(Back, [this, player](Ref *ref){
        player->stopBackgroundMusic();
        player->playBackgroundMusic("audio/menu_bgm.MP3", true);
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4f, SelectLevelScene::createScene()));
    });
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    //create map
    float x = origin.x + 350 + SIZE;
    float y = origin.y + visibleSize.height - SIZE;
    createMap(1);
    for(size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            Grid::Type type_ = map->grids[map->shape.index_of(i, j)].type;
            if(type_ != Grid::Type::None) {
                grid[i][j] = ui::Button::create(images[type_], images[type_]);
                grid[i][j]->setPosition(Vec2(x + j * SIZE, y - i * SIZE));
                this->addChild(grid[i][j], 2);
            }
        }
    }
    
    auto blockBackground = Sprite::create("images/block_background.png", Rect(0, 0, 1680, 980));
    if(blockBackground == nullptr) {
        problemLoading("'images/block_background.png'");
    } else {
        blockBackground->setPosition(Vec2(x + 5.5f * SIZE, y - 3 * SIZE));
        this->addChild(blockBackground, 1);
    }
    
    auto money = Sprite::create("images/gold.png");
    if(money == nullptr) {
        problemLoading("'images/gold.png'");
    } else {
        money->setPosition(Vec2(origin.x + 70, origin.y + visibleSize.height - 70));
        this->addChild(money, 1);
    }
    this->moneyLabel = cocos2d::Label::createWithTTF(std::to_string(this->map->cost_), "fonts/Bender/BENDER.OTF", 75);
    this->moneyLabel->setPosition(cocos2d::Vec2(origin.x + 150 + 15 * log10(this->map->cost_), origin.y + visibleSize.height - 70));
    this->addChild(this->moneyLabel, 1);
    
    // upgrade menu
    this->upgradeBackground1 = Sprite::create("images/upgrade_background.png");
    if(this->upgradeBackground1 == nullptr) {
        problemLoading("'images/upgrade_background.png'");
    } else {
        this->upgradeBackground1->setPosition(Vec2(origin.x + visibleSize.width / 2 - 800, origin.y + visibleSize.height / 2));
        this->addChild(this->upgradeBackground1, 5);
        this->upgradeBackground1->setVisible(false);
    }
    this->upgradeBackground2 = Sprite::create(
            "images/upgrade_background.png");
    if(this->upgradeBackground2 == nullptr) {
        problemLoading("'images/upgrade_background.png'");
    } else {
        this->upgradeBackground2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(this->upgradeBackground2, 5);
        this->upgradeBackground2->setVisible(false);
    }
    this->upgradeBackground3 = Sprite::create("images/upgrade_background.png");
    if(this->upgradeBackground3 == nullptr) {
        problemLoading("'images/upgrade_background.png'");
    } else {
        this->upgradeBackground3->setPosition(Vec2(origin.x + visibleSize.width / 2 + 800, origin.y + visibleSize.height / 2));
        this->addChild(this->upgradeBackground3, 5);
        this->upgradeBackground3->setVisible(false);
    }
    
    this->upgradeTower1 = Sprite::create("images/towers/archer_base.png");
    if(this->upgradeTower1 == nullptr) {
        problemLoading("'images/towers/archer.png'");
    } else {
        this->upgradeTower1->setPosition(Vec2(origin.x + visibleSize.width / 2 - 800, origin.y + visibleSize.height / 2 + 150));
        this->addChild(this->upgradeTower1, 6);
        this->upgradeTower1->setVisible(false);
    }
    this->upgradeTower2 = Sprite::create("images/towers/magician_base.png");
    if(this->upgradeTower2 == nullptr) {
        problemLoading("'images/towers/bomber.png'");
    } else {
        this->upgradeTower2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 150));
        this->addChild(this->upgradeTower2, 6);
        this->upgradeTower2->setVisible(false);
    }
    this->upgradeTower3 = Sprite::create("images/towers/helper_base.png");
    if(this->upgradeTower3 == nullptr) {
        problemLoading("'images/towers/highspeed_archer.png'");
    } else {
        this->upgradeTower3->setPosition(Vec2(origin.x + visibleSize.width / 2 + 800, origin.y + visibleSize.height / 2 + 150));
        this->addChild(this->upgradeTower3, 6);
        this->upgradeTower3->setVisible(false);
    }
    
    auto upgradeLabel1 = Label::createWithTTF("Choose", "fonts/Bender/BENDER.OTF", 75);
    this->upgradeItem1 = MenuItemLabel::create(upgradeLabel1, [this](Ref *ref){
        Sprite *towerSprite = this->getTower(this->selectedTowerId);
        std::unique_ptr<TowerFactoryBase> newTower;
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float typeX = origin.x + 350 + SIZE;
        float typeY = origin.y + visibleSize.height - SIZE;
        float x = towerSprite->getPositionX();
        float y = towerSprite->getPositionY();
        int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
        std::string path;
        switch (this->map->get_ref(indexY, indexX).grid.tower.value()->status().tower_type_) {
            case TowerType::ArcherBase:
                path = "images/towers/archer.png";
                newTower = std::make_unique<TowerFactory<Archer>>();
                break;
            case TowerType::MagicianBase:
                path = "images/towers/core_magician.png";
                newTower = std::make_unique<TowerFactory<CoreMagician>>();
                break;
            case TowerType::HelperBase:
                path = "images/towers/decelerate_magician.png";
                newTower = std::make_unique<TowerFactory<DecelerateMagician>>();
                break;
            default:
                std::unreachable();
                break;
        }
        this->deleteTower(false);
        auto id = this->map->spawn_tower_at(indexY, indexX, *newTower);
        auto newTowerSprite = Sprite::create(path);
        newTowerSprite->setPosition(Vec2(x, y));
        this->addChild(newTowerSprite, 3);
        this->selectedTowerId = id.value();
        this->towers.emplace_back(id.value(), newTowerSprite);
        this->moneyLabel->setString(std::to_string(this->map->cost_));
        this->updateSelectorEnabled();
        this->hideUpgradeMenu();
    });
    upgradeItem1->setPosition(Vec2(origin.x + visibleSize.width / 2 - 800, origin.y + visibleSize.height / 2 - 150));
    auto upgradeLabel2 = Label::createWithTTF("Choose", "fonts/Bender/BENDER.OTF", 75);
    this->upgradeItem2 = MenuItemLabel::create(upgradeLabel2, [this](Ref *ref){
        Sprite *towerSprite = this->getTower(this->selectedTowerId);
        std::unique_ptr<TowerFactoryBase> newTower;
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float typeX = origin.x + 350 + SIZE;
        float typeY = origin.y + visibleSize.height - SIZE;
        float x = towerSprite->getPositionX();
        float y = towerSprite->getPositionY();
        int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
        std::string path;
        switch (this->map->get_ref(indexY, indexX).grid.tower.value()->status().tower_type_) {
            case TowerType::ArcherBase:
                path = "images/towers/highspeed_archer.png";
                newTower = std::make_unique<TowerFactory<HighspeedArcher>>();
                break;
            case TowerType::MagicianBase:
                path = "images/towers/diffusive_magician.png";
                newTower = std::make_unique<TowerFactory<DiffusiveMagician>>();
                break;
            case TowerType::HelperBase:
                path = "images/towers/weaken_magician.png";
                newTower = std::make_unique<TowerFactory<WeakenMagician>>();
                break;
            default:
                break;
        }
        this->deleteTower(false);
        auto id = this->map->spawn_tower_at(indexY, indexX, *newTower);
        auto newTowerSprite = Sprite::create(path);
        newTowerSprite->setPosition(Vec2(x, y));
        this->addChild(newTowerSprite, 3);
        this->selectedTowerId = id.value();
        this->towers.emplace_back(id.value(), newTowerSprite);
        this->moneyLabel->setString(std::to_string(this->map->cost_));
        this->updateSelectorEnabled();
        this->hideUpgradeMenu();
    });
    upgradeItem2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 150));
    auto upgradeLabel3 = Label::createWithTTF("Choose", "fonts/Bender/BENDER.OTF", 75);
    this->upgradeItem3 = MenuItemLabel::create(upgradeLabel3, [this](Ref *ref){
        Sprite *towerSprite = this->getTower(this->selectedTowerId);
        std::unique_ptr<TowerFactoryBase> newTower;
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float typeX = origin.x + 350 + SIZE;
        float typeY = origin.y + visibleSize.height - SIZE;
        float x = towerSprite->getPositionX();
        float y = towerSprite->getPositionY();
        int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
        std::string path;
        switch (this->map->get_ref(indexY, indexX).grid.tower.value()->status().tower_type_) {
            case TowerType::ArcherBase:
                path = "images/towers/bomber.png";
                newTower = std::make_unique<TowerFactory<Bomber>>();
                break;
            case TowerType::MagicianBase:
                path = "images/towers/special_magician.png";
                newTower = std::make_unique<TowerFactory<SpecialMagician>>();
                break;
            case TowerType::HelperBase:
                path = "images/towers/aggressive_magician.png";
                newTower = std::make_unique<TowerFactory<AggressiveMagician>>();
                break;
            default:
                break;
        }
        this->deleteTower(false);
        auto id = this->map->spawn_tower_at(indexY, indexX, *newTower);
        auto newTowerSprite = Sprite::create(path);
        newTowerSprite->setPosition(Vec2(x, y));
        this->addChild(newTowerSprite, 3);
        this->selectedTowerId = id.value();
        this->towers.emplace_back(id.value(), newTowerSprite);
        this->moneyLabel->setString(std::to_string(this->map->cost_));
        this->updateSelectorEnabled();
        this->hideUpgradeMenu();
    });
    upgradeItem3->setPosition(Vec2(origin.x + visibleSize.width / 2 + 800, origin.y + visibleSize.height / 2 - 150));
    auto cancelLabel = Label::createWithTTF("Cancel", "fonts/Bender/BENDER.OTF", 75);
    cancelLabel->setColor(Color3B(255, 0, 0));
    this->cancelUpgradeItem = MenuItemLabel::create(cancelLabel, [this](Ref *ref){
        this->hideUpgradeMenu();
    });
    this->cancelUpgradeItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 300));
    
    auto enemyExample = Sprite::create("images/enemies/dog/move/dog_move00.png");
    if (enemyExample == nullptr) {
        problemLoading("'images/enemies/dog/move/dog_move00.png'");
    } else {
        enemyExample->setPosition(Vec2(origin.x, origin.y + visibleSize.height / 2));
        enemyExample->setScale(0.25f);
        this->addChild(enemyExample, 5);
        Vector<SpriteFrame*> frames;
        int num = 15;
        frames.reserve(num);
        for (int i = 0; i < num; i++) {
            std::string path = std::format("images/enemies/dog/move/dog_move{:02d}.png", i);
            frames.pushBack(SpriteFrame::create(path, Rect(0, 0, 900, 900)));
        }
        Animation* animation = Animation::createWithSpriteFrames(frames, 0.05f);
        Animate* animate = Animate::create(animation);
        
        auto moveBy = MoveBy::create(0.8f, Vec2(140, 0));
        auto spawn = Spawn::create(moveBy, animate, nullptr);
        enemyExample->runAction(RepeatForever::create(spawn));
    }
    
    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, 2);
    menu->setPosition(Vec2::ZERO);
    
    Vector<MenuItem*> upgradeMenuItems;
    upgradeMenuItems.pushBack(this->upgradeItem1);
    upgradeMenuItems.pushBack(this->upgradeItem2);
    upgradeMenuItems.pushBack(this->upgradeItem3);
    upgradeMenuItems.pushBack(this->cancelUpgradeItem);
    this->upgradeMenu = Menu::createWithArray(upgradeMenuItems);
    this->addChild(this->upgradeMenu, 6);
    this->upgradeMenu->setPosition(Vec2::ZERO);
    this->upgradeMenu->setVisible(false);
    
    // add a mouse click event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Level1Scene::onMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(Level1Scene::onMouseMove, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(Level1Scene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // update the map
    schedule([this](float dt){
        map->update();
        this->update();
    }, 1.0/30, "update");

    return true;
}