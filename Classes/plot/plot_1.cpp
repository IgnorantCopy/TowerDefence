#include "plot_1.h"
#include "SelectLevelScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene *plot_1::createScene() { return plot_1::create(); }

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in HelloWorldScene.cpp\n");
}

bool plot_1::init() {
    if (!Scene::init())
        return false;
    plot::init();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::string filepath =
        FileUtils::getInstance()->fullPathForFilename("plot/plot_1.txt");
    std::string content = FileUtils::getInstance()->getStringFromFile(filepath);
    auto plot = Label::createWithTTF(
        "", "fonts/manual_font/SourceHanSerifCN-Bold-2.otf", 30);
    plot->setString(content);
    plot->setColor(Color3B::WHITE);
    plot->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    this->addChild(plot, 1);

    return true;
}
