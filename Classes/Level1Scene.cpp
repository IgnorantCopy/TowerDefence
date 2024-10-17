#include "Level1Scene.h"
#include "SelectLevelScene.h"
#include "cocostudio/SimpleAudioEngine.h"
#include "core/entity/tower/aggressive_magician.h"
#include "core/entity/tower/archer.h"
#include "core/entity/tower/bomber.h"
#include "core/entity/tower/core_magician.h"
#include "core/entity/tower/decelerate_magician.h"
#include "core/entity/tower/diffusive_magician.h"
#include "core/entity/tower/highspeed_archer.h"
#include "core/entity/tower/special_magician.h"
#include "core/entity/tower/weaken_magician.h"

USING_NS_CC;
using towerdefence::core::AggressiveMagician;
using towerdefence::core::Archer;
using towerdefence::core::Bomber;
using towerdefence::core::CoreMagician;
using towerdefence::core::DecelerateMagician;
using towerdefence::core::DiffusiveMagician;
using towerdefence::core::HighspeedArcher;
using towerdefence::core::SpecialMagician;
using towerdefence::core::TowerFactory;
using towerdefence::core::TowerFactoryBase;
using towerdefence::core::TowerType;
using towerdefence::core::WeakenMagician;

Scene *Level1Scene::createScene() { return Level1Scene::create(); }

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in Level1Scene.cpp\n");
}

bool Level1Scene::init() {
    LevelScene::init();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add bgm
    auto player = CocosDenshion::SimpleAudioEngine::getInstance();
    player->stopBackgroundMusic();
    player->playBackgroundMusic("audio/level1_bgm.MP3", true);

    // the back button to go back to the SelectLevel scene
    auto Back = Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem = MenuItemLabel::create(Back, [this, player](Ref *ref) {
        player->stopBackgroundMusic();
        player->playBackgroundMusic("audio/menu_bgm.MP3", true);
        Director::getInstance()->replaceScene(
            TransitionCrossFade::create(0.4f, SelectLevelScene::createScene()));
    });
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    Vector<MenuItem *> menuItems;
    menuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}