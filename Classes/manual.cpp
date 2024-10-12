#include "manual.h"
#include "ui/CocosGUI.h"
#include "archer_base.h"
#include "highspeed_archor.h"
#include "highspeed_archor_p.h"
#include "bomber.h"
#include "bomber_p.h"
#include "archer.h"
#include "archer_p.h"
#include "magician_base.h"
#include "core_magician.h"
#include "core_magician_p.h"
#include "diffusive_magician.h"
#include "diffusive_magician_p.h"
#include "special_magician.h"
#include "special_magician_p.h"
#include "helper_base.h"
#include "decelerate_magician.h"
#include "decelerate_magician_p.h"
#include "weaken_magician.h"
#include "weaken_magician_p.h"
#include "aggressive_magician.h"
#include "aggressive_magician_p.h"

USING_NS_CC;

Scene* manual::createScene() {
    Scene* scene = Scene::create();
    return scene;
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool manual::init() {
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

    auto archer_base=MenuItemImage::create(
            "images/towers/archer_base.png",
            "images/towers/archer_base.png",
            [this](Ref *ref){
                Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4f, archer_base::createScene()));
            }
    );
    archer_base->setPosition(Vec2(origin.x + visibleSize.width / 2 - 800,
                               origin.y + visibleSize.height / 2));
    return true;
}