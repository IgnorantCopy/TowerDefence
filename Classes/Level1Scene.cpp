#include "Level1Scene.h"

USING_NS_CC;

Scene *Level1Scene::createScene() {
    return Level1Scene::create();
}

bool Level1Scene::init() {
    return LevelScene::init(1);
}