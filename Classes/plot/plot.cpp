//
// Created by Ignorant on 2024/10/21.
//

#include "plot.h"

#include "SelectLevelScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in HelloWorldScene.cpp\n");
}

bool plot::init() {
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

    this->backgroundOver =
        Sprite::create("images/white_background.png", Rect(0, 0, 2500, 1500));
    this->backgroundOver->setPosition(visibleSize.width / 2 + origin.x,
                                      visibleSize.height / 2 + origin.y);
    this->backgroundOver->setColor(Color3B(128, 128, 128));
    this->backgroundOver->setVisible(false);
    this->addChild(this->backgroundOver, 2);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(plot::onMouseDown, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(plot::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener,
                                                             this);

    return true;
}

void plot::showSecret() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto move = MoveTo::create(2.0f, Vec2(origin.x + visibleSize.width / 2,
                                          origin.y + visibleSize.height / 2));
    auto moveEase = EaseBackOut::create(move);
    this->secret->setVisible(true);
    this->secret->setScale(1.0f);
    this->secret->runAction(moveEase);

    auto fadeTo = FadeTo::create(0.5f, 125);
    this->backgroundOver->setVisible(true);
    this->backgroundOver->setOpacity(0);
    this->backgroundOver->runAction(fadeTo);
    this->isShowingSecret = true;
}

void plot::hideSecret() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto scaleTo = ScaleTo::create(0.4f, 0.1f);
    auto callback = CallFunc::create([this, origin, visibleSize]() {
        this->secret->setVisible(false);
        this->secret->setPosition(
            Vec2(origin.x + visibleSize.width / 2,
                 origin.y + visibleSize.height +
                     this->secret->getContentSize().height / 2));
        this->backgroundOver->setVisible(false);
        this->isShowingSecret = false;
    });
    auto seq = Sequence::create(scaleTo, callback, nullptr);
    this->secret->runAction(seq);
}

void plot::onMouseDown(cocos2d::Event *event) {
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

void plot::onMouseUp(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *)event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (x > origin.x + visibleSize.width - 200 &&
        x < origin.x + visibleSize.width && y > origin.y &&
        y < origin.y + 200 && !this->isShowingSecret &&
        this->secret != nullptr) {
        this->showSecret();
    } else if (this->secret != nullptr &&
               (x < origin.x + visibleSize.width / 2 -
                        this->secret->getContentSize().width / 2 ||
                x > origin.x + visibleSize.width / 2 +
                        this->secret->getContentSize().width / 2 ||
                y < origin.y + visibleSize.height / 2 -
                        this->secret->getContentSize().height / 2 ||
                y > origin.y + visibleSize.height / 2 +
                        this->secret->getContentSize().height / 2) &&
               this->isShowingSecret) {
        this->hideSecret();
    }
}
