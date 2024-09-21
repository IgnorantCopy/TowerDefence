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
#include "SelectLevelScene.h"
#include "Gif/GIFMovie.h"
#include "Gif/CacheGif.h"
#include "Gif/InstantGif.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() ) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    auto labelPlay = Label::createWithTTF("Play", "fonts/Bender/BENDER.OTF", 100);
    auto playItem =  MenuItemLabel::create(
            labelPlay,
            [this](Ref *ref){
                log("create PlayScene");//TODO: create a playScene
                Director::getInstance()->replaceScene(SelectLevelScene::createScene());
            }
    );
    playItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height / 2));
    
    auto labelManual = Label::createWithTTF("Manual", "fonts/Bender/BENDER.OTF", 100);
    auto manualItem = MenuItemLabel::create(
            labelManual,
            [this](Ref *ref){
                log("create ManualScene");//TODO: create a manualScene
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
    }

    auto background = Sprite::create("images/menu_background_copy.png", Rect(0, 0, 2500, 1500));
    if (background == nullptr) {
        problemLoading("'images/menu_background_copy.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
        background->setOpacity(0);
        auto fadeIn = FadeIn::create(3.0f);
        background->runAction(fadeIn);
    }
    
    // create menu, it's an autorelease object
    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(playItem);
    MenuItems.pushBack(quitItem);
    MenuItems.pushBack(manualItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, 2);
    menu->setPosition(Vec2::ZERO);
    
    // animation for menu
    menu->setOpacity(0);
    auto delay1 = DelayTime::create(3.8f);
    auto scaleDown = ScaleBy::create(0.1f, 0.1f);
    auto fadeIn = FadeIn::create(0.1f);
    auto scaleUp = ScaleBy::create(0.5f, 10.0f);
    auto scaleEase = EaseBackOut::create(scaleUp->clone());
    auto delay2 = DelayTime::create(0.8f);
    auto seq = Sequence::create(delay1, scaleDown, fadeIn, delay2, scaleEase, NULL);
    menu->runAction(seq);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}
