#include "illustration.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* illustration::createScene() {
    Scene* scene = Scene::create();
    return scene;
}

bool illustration::init() {
    if(!Scene::init())
        return false;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

}