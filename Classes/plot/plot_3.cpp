#include "plot_3.h"
#include "SelectLevelScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene *plot_3::createScene() { return plot_3::create(); }

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in HelloWorldScene.cpp\n");
}

bool plot_3::init() {
    if (!Scene::init())
        return false;
    plot::init();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->secret = Sprite::create("images/secret2.png");
    this->secret->setPosition(
        Vec2(origin.x + visibleSize.width / 2,
             origin.y + visibleSize.height +
                 this->secret->getContentSize().height / 2));
    this->secret->setVisible(false);
    this->addChild(this->secret, 3);

    std::string filepath =
        FileUtils::getInstance()->fullPathForFilename("plot/plot_3.txt");
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