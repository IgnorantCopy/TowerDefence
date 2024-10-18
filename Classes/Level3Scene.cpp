#include "Level3Scene.h"

USING_NS_CC;

Scene* Level3Scene::createScene() {
    return Level3Scene::create();
}

bool Level3Scene::init() {
    return LevelScene::init(3);
}