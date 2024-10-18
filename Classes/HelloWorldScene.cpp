/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "2d/CCLabel.h"
#include "SelectLevelScene.h"
#include "manual/manual.h"
#include "cocostudio/SimpleAudioEngine.h"

USING_NS_CC;

bool firstTouch = true;

Scene *HelloWorld::createScene() {
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (firstTouch) {
        // add bgm
        auto player = CocosDenshion::SimpleAudioEngine::getInstance();
        player->playBackgroundMusic("audio/menu_bgm.mp3", true);
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    auto labelPlay = Label::createWithTTF("Play", "fonts/Bender/BENDER.OTF", 100);
    auto playItem = MenuItemLabel::create(
            labelPlay,
            [this](Ref *ref) {
                Director::getInstance()->replaceScene(
                        TransitionCrossFade::create(0.5f, SelectLevelScene::createScene()));
            }
    );
    playItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 2));

    auto labelManual = Label::createWithTTF("Manual", "fonts/Bender/BENDER.OTF", 100);
    auto manualItem = MenuItemLabel::create(
            labelManual,
            [this](Ref *ref) {
                Director::getInstance()->pushScene(TransitionTurnOffTiles::create(0.5f, manual::createScene()));
            }
    );
    manualItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                 origin.y + visibleSize.height / 2 - 200));

    auto labelQuit = Label::createWithTTF("Quit", "fonts/Bender/BENDER.OTF", 100);
    auto quitItem = MenuItemLabel::create(
            labelQuit,
            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
    );
    quitItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 2 - 400));
    /////////////////////////////
    // 3. add your codes below...

    // add a labelTitle shows "Hello World"
    // create and initialize a labelTitle

    auto labelTitle = Label::createWithTTF("Tower Defence", "fonts/Bender/BENDER.OTF", 200);
    if (labelTitle == nullptr) {
        problemLoading("'fonts/Bender/BENDER.OTF'");
    } else {
        // position the labelTitle on the center of the screen
        if (firstTouch) {
            labelTitle->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                         origin.y + visibleSize.height / 2));
            labelTitle->setOpacity(0);
            // add the labelTitle as a child to this layer
            this->addChild(labelTitle, 1);
            auto delay = DelayTime::create(1);
            auto fadeIn = FadeIn::create(3.0f);
            auto move = MoveBy::create(1, Vec2(0, 400));
            auto move_ease = EaseBackOut::create(move->clone());
            auto seq = Sequence::create(fadeIn, delay, move_ease, NULL);
            labelTitle->runAction(seq);
        } else {
            labelTitle->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                         origin.y + visibleSize.height / 2 + 400));
            this->addChild(labelTitle, 1);
        }
    }

    auto background = Sprite::create("images/menu_background.png", Rect(0, 0, 2500, 1500));
    if (background == nullptr) {
        problemLoading("'images/menu_background.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
        if (firstTouch) {
            background->setOpacity(0);
            auto fadeIn = FadeIn::create(3.0f);
            background->runAction(fadeIn);
        }
    }

    // create menu, it's an autorelease object
    Vector<MenuItem *> MenuItems;
    MenuItems.pushBack(playItem);
    MenuItems.pushBack(quitItem);
    MenuItems.pushBack(manualItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, 2);
    menu->setPosition(Vec2::ZERO);

    // animation for menu
    if (firstTouch) {
        menu->setVisible(false);
        menu->setScale(0.1f);
        auto delay = DelayTime::create(4.8f);
        auto scaleUp = ScaleBy::create(0.5f, 10.0f);
        auto scaleEase = EaseBackOut::create(scaleUp->clone());
        auto seq = Sequence::create(delay, scaleEase, NULL);
        menu->runAction(seq);
        scheduleOnce([this, menu, scaleEase](float dt) {
            menu->setVisible(true);
        }, 4.8f, "animateMenu");
    }
    if (firstTouch) {
        firstTouch = false;
    }

    // add a mouse click event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void HelloWorld::onMouseDown(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *) event;
    float x = e->getCursorX();
    float y = e->getCursorY();

    auto particle = ParticleSystemQuad::create("particles/mouse.plist");
    if (particle == nullptr) {
        problemLoading("'particles/mouse.plist'");
    } else {
        particle->setPosition(Vec2(x, y));
        this->addChild(particle, 3);
    }
}
