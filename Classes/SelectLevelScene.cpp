#include "SelectLevelScene.h"
#include "HelloWorldScene.h"
#include "Gif/GIFMovie.h"
#include "Gif/CacheGif.h"
#include "Gif/InstantGif.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* SelectLevelScene::createScene()
{
    return SelectLevelScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool SelectLevelScene::init()
{
    if( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(SelectLevelScene::menuCloseCallback, this));
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // the background image
    auto background=Sprite::create("images/sellect_background.jpg", Rect(0, 0, 2500, 1500));
    if (background == nullptr) {
        problemLoading("'images/sellect_background.jpg'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }

    // the back button to go back to the HelloWorld scene
    auto Back=Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem=MenuItemLabel::create(
            Back,
            [this](Ref *ref){
                Director::getInstance()->popSceneWithTransition<TransitionCrossFade>(0.4f);
            }
    );
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    auto level1=MenuItemImage::create(
            "images/in.png",
            "images/in.png",
            [this](Ref *ref){
                // TODO: fill the below code with the class level1
                //Director::getInstance()->replaceScene(*****::createScene());
            }
    );
    level1->setPosition(Vec2(origin.x + visibleSize.width / 2 - 600,
                               origin.y + visibleSize.height / 2 + 300));
    // TODO: the animation of the Level1 button

    auto level2=MenuItemImage::create(
            "images/in.png",
            "images/in.png",
            [this](Ref *ref){
                // TODO: fill the below code with the class level2
                //Director::getInstance()->replaceScene(*****::createScene());
            }
    );
    level2->setPosition(Vec2(origin.x + visibleSize.width / 2,
                             origin.y + visibleSize.height / 2 + 300));
    // TODO: the animation of the Level2 button

    auto level3=MenuItemImage::create(
            "images/in.png",
            "images/in.png",
            [this](Ref *ref){
                // TODO: fill the below code with the class level3
                //Director::getInstance()->replaceScene(*****::createScene());
            }
    );
    level3->setPosition(Vec2(origin.x + visibleSize.width / 2 + 600,
                             origin.y + visibleSize.height / 2 + 300));
    // TODO: the animation of the Level3 button

    // create menu, it's an autorelease object
    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    MenuItems.pushBack(level1);
    MenuItems.pushBack(level2);
    MenuItems.pushBack(level3);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);

    return true;
}

void SelectLevelScene::menuCloseCallback(cocos2d::Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}