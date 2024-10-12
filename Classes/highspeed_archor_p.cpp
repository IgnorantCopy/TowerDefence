#include "highspeed_archor_p.h"

USING_NS_CC;

Scene* highspeed_archor_p::createScene() {
    Scene* scene = Scene::create();
    return scene;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool highspeed_archor_p::init() {
    if(!Scene::init())
        return false;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background=Sprite::create("images/manual_background.png", Rect(0, 0, 2500, 1500));
    if (background == nullptr) {
        problemLoading("'images/manual_background.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }

    auto Back=Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem=MenuItemLabel::create(
            Back,
            [this](Ref *ref){
                Director::getInstance()->popSceneWithTransition<TransitionCrossFade>(0.4f);
            }
    );
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    auto picture = Sprite::create("images/manual/highspeed_archer_p.png", Rect(0, 0, 2500, 1500));
    if (picture == nullptr) {
        problemLoading("'images/manual/highspeed_archer_p.png'");
    } else {
        picture->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(picture, 0);
    }

    return true;
}