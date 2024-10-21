#include "plot_1.h"
#include "SelectLevelScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene *plot_1::createScene() { return plot_1::create(); }

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in HelloWorldScene.cpp\n");
}

bool plot_1::init() {
    if (!Scene::init())
        return false;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto Back = Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem = MenuItemLabel::create(Back, [this](Ref *ref) {
        Director::getInstance()->replaceScene(
            TransitionCrossFade::create(0.4f, SelectLevelScene::createScene()));
    });
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));
    Vector<MenuItem *> MenuItems;
    MenuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);

    auto particle = ParticleSystemQuad::create("particles/secret.plist");
    particle->setPosition(
        Vec2(origin.x + visibleSize.width - 100, origin.y + 100));
    particle->setScale(0.5f);
    this->addChild(particle, 1);

    std::string filepath =
        FileUtils::getInstance()->fullPathForFilename("plot/plot_1.txt");
    std::string content = FileUtils::getInstance()->getStringFromFile(filepath);
    auto plot = Label::createWithTTF(
        "", "fonts/manual_font/SourceHanSerifCN-Bold-2.otf", 30);
    plot->setString(content);
    plot->setColor(Color3B::WHITE);
    plot->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    this->addChild(plot, 1);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(plot_1::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(plot_1::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener,
                                                             this);

    return true;
}

void plot_1::onMouseDown(cocos2d::Event *event) {
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

void plot_1::onMouseUp(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *)event;
    float x = e->getCursorX();
    float y = e->getCursorY();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (x > origin.x + visibleSize.width - 200 &&
        x < origin.x + visibleSize.width &&
        y > origin.y + visibleSize.height - 50 &&
        y < origin.y + visibleSize.height) {
    }