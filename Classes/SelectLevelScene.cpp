#include "SelectLevelScene.h"
#include "HelloWorldScene.h"
#include "Level1Scene.h"
#include "Level2Scene.h"
#include "Level3Scene.h"
#include "cocostudio/SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene *SelectLevelScene::createScene() { return SelectLevelScene::create(); }

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in SelectLevelScene.cpp\n");
}

bool SelectLevelScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto userDefault = UserDefault::getInstance();
    int level1Scene = userDefault->getIntegerForKey("level1", 0);
    int level2Scene = userDefault->getIntegerForKey("level2", 4);
    int level3Scene = userDefault->getIntegerForKey("level3", 4);

    // add bgm
    auto player = CocosDenshion::SimpleAudioEngine::getInstance();
    player->setBackgroundMusicVolume(float(userDefault->getIntegerForKey("musicVolume", 100)) / 100.0f);
    player->setEffectsVolume(float(userDefault->getIntegerForKey("effectVolume", 100)) / 100.0f);

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(SelectLevelScene::menuCloseCallback, this));
    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width -
                  closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // the background image
    auto background =
        Sprite::create("images/select_background.jpg", Rect(0, 0, 2500, 1500));
    if (background == nullptr) {
        problemLoading("'images/select_background.jpg'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                     origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }

    // the back button to go back to the HelloWorld scene
    auto Back = Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem = MenuItemLabel::create(Back, [this](Ref *ref) {
        Director::getInstance()->replaceScene(
            TransitionCrossFade::create(0.4f, HelloWorld::createScene()));
    });
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    auto level1Button = ui::Button::create(
        "images/level1_background_select.png",
        "images/level1_background_select.png", "images/locked.png");
    level1Button->addTouchEventListener(
        [this, player](Ref *pSender, ui::Widget::TouchEventType type) {
            switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                player->stopBackgroundMusic();
                Director::getInstance()->replaceScene(
                    TransitionCrossFade::create(0.4f,
                                                Level1Scene::createScene()));
                break;
            default:
                break;
            }
        });
    level1Button->setPosition(Vec2(origin.x + visibleSize.width / 2 - 800,
                                   origin.y + visibleSize.height / 2));

    auto level2Button = ui::Button::create(
        "images/level2_background_select.png",
        "images/level2_background_select.png", "images/locked.png");
    level2Button->addTouchEventListener(
        [this, player](Ref *pSender, ui::Widget::TouchEventType type) {
            switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                player->stopBackgroundMusic();
                Director::getInstance()->replaceScene(
                    TransitionCrossFade::create(0.4f,
                                                Level2Scene::createScene()));
                break;
            default:
                break;
            }
        });
    level2Button->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                   origin.y + visibleSize.height / 2));

    auto level3Button = ui::Button::create(
        "images/level3_background_select.png",
        "images/level3_background_select.png", "images/locked.png");
    level3Button->addTouchEventListener(
        [this, player](Ref *pSender, ui::Widget::TouchEventType type) {
            switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                player->stopBackgroundMusic();
                Director::getInstance()->replaceScene(
                    TransitionCrossFade::create(0.4f,
                                                Level3Scene::createScene()));
                break;
            default:
                break;
            }
        });
    level3Button->setPosition(Vec2(origin.x + visibleSize.width / 2 + 800,
                                   origin.y + visibleSize.height / 2));

    switch (level1Scene) {
    case 1:
        level1Star = Sprite::create("images/bronze.png");
        level1Star->setPosition(Vec2(origin.x + visibleSize.width / 2 - 600,
                                    origin.y + visibleSize.height / 2 + 400));
        this->addChild(level1Star, 5);
        break;
    case 2:
        level1Star = Sprite::create("images/silver.png");
        level1Star->setPosition(Vec2(origin.x + visibleSize.width / 2 - 600,
                                    origin.y + visibleSize.height / 2 + 400));
        this->addChild(level1Star, 5);
        break;
    case 3:
        level1Star = Sprite::create("images/golden.png");
        level1Star->setPosition(Vec2(origin.x + visibleSize.width / 2 - 600,
                                    origin.y + visibleSize.height / 2 + 400));
        this->addChild(level1Star, 5);
        break;
    case 0:
        break;
    default:
        level1Button->setEnabled(false);
        break;
    }
    switch (level2Scene) {
    case 1:
        level2Star = Sprite::create("images/bronze.png");
        level2Star->setPosition(Vec2(origin.x + visibleSize.width / 2 + 200,
                                     origin.y + visibleSize.height / 2 + 400));
        this->addChild(level2Star, 5);
        break;
    case 2:
        level2Star = Sprite::create("images/silver.png");
        level2Star->setPosition(Vec2(origin.x + visibleSize.width / 2 + 200,
                                     origin.y + visibleSize.height / 2 + 400));
        this->addChild(level2Star, 5);
        break;
    case 3:
        level2Star = Sprite::create("images/golden.png");
        level2Star->setPosition(Vec2(origin.x + visibleSize.width / 2 + 200,
                                     origin.y + visibleSize.height / 2 + 400));
        this->addChild(level2Star, 5);
        break;
    case 0:
        break;
    default:
        level2Button->setEnabled(false);
        break;
    }
    switch (level3Scene) {
    case 1:
        level3Star = Sprite::create("images/bronze.png");
        level3Star->setPosition(Vec2(origin.x + visibleSize.width / 2 + 1000,
                                     origin.y + visibleSize.height / 2 + 400));
        this->addChild(level3Star, 5);
        break;
    case 2:
        level3Star = Sprite::create("images/silver.png");
        level3Star->setPosition(Vec2(origin.x + visibleSize.width / 2 + 1000,
                                     origin.y + visibleSize.height / 2 + 400));
        this->addChild(level3Star, 5);
        break;
    case 3:
        level3Star = Sprite::create("images/golden.png");
        level3Star->setPosition(Vec2(origin.x + visibleSize.width / 2 + 1000,
                                     origin.y + visibleSize.height / 2 + 400));
        this->addChild(level3Star, 5);
        break;
    case 0:
        break;
    default:
        level3Button->setEnabled(false);
        break;
    }
    this->addChild(level1Button, 1);
    this->addChild(level2Button, 1);
    this->addChild(level3Button, 1);

    // create menu, it's an autorelease object
    Vector<MenuItem *> MenuItems;
    MenuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);

    // add a mouse click event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown =
        CC_CALLBACK_1(SelectLevelScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener,
                                                             this);

    return true;
}

void SelectLevelScene::menuCloseCallback(cocos2d::Ref *pSender) {
    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the
     * application  ,do not use Director::getInstance()->end() as given
     * above,instead trigger a custom event created in RootViewController.mm as
     * below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void SelectLevelScene::onMouseDown(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *)event;
    float x = e->getCursorX();
    float y = e->getCursorY();

    auto particle = ParticleSystemQuad::create("particles/mouse.plist");
    if (particle == nullptr) {
        problemLoading("'particles/mouse.plist'");
    } else {
        particle->setPosition(Vec2(x, y));
        this->addChild(particle, 5);
    }
}