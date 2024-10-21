#include "plot_4.h"
#include "SelectLevelScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene *plot_4::createScene() { return plot_4::create(); }

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in HelloWorldScene.cpp\n");
}

bool plot_4::init() {
    if (!Scene::init())
        return false;
    plot::init();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->secret = Sprite::create("images/secret3.png");
    this->secret->setPosition(
        Vec2(origin.x + visibleSize.width / 2,
             origin.y + visibleSize.height +
                 this->secret->getContentSize().height / 2));
    this->secret->setVisible(false);
    this->addChild(this->secret, 3);

    std::string filepath =
        FileUtils::getInstance()->fullPathForFilename("plot/plot_4.txt");
    std::string content = FileUtils::getInstance()->getStringFromFile(filepath);
    auto plot = Label::createWithTTF(
        "", "fonts/manual_font/SourceHanSerifCN-Bold-2.otf", 30);
    plot->setString(content);
    plot->setColor(Color3B::WHITE);
    plot->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    this->addChild(plot, 1);

    auto sentence1 = Label::createWithTTF("I'm waiting for you",
                                          "fonts/EndfieldByButan.ttf", 30);
    sentence1->setColor(Color3B::WHITE);
    sentence1->setPosition(Vec2(origin.x + visibleSize.width / 2 - 575,
                                origin.y + visibleSize.height / 2 - 310));
    this->addChild(sentence1, 1);

    auto sentence2 = Label::createWithTTF("I'm coming to see you",
                                          "fonts/EndfieldByButan.ttf", 30);
    sentence2->setColor(Color3B::WHITE);
    sentence2->setPosition(Vec2(origin.x + visibleSize.width / 2 - 575,
                                origin.y + visibleSize.height / 2 - 445));
    this->addChild(sentence2, 1);
    return true;
}