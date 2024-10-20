#include "Level2Scene.h"

USING_NS_CC;

Scene* Level2Scene::createScene() {
    return Level2Scene::create();
}

bool Level2Scene::init() {
    return LevelScene::init(2);
}