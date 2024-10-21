#include "plot_3.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "SelectLevelScene.h"

USING_NS_CC;

Scene* plot_3::createScene() {
    return plot_3::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool plot_3::init() {
    if(!Scene::init())
        return false;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto Back=Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem=MenuItemLabel::create(
            Back,
            [this](Ref *ref){
                Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4f,SelectLevelScene::createScene()));
            }
    );
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,origin.y + visibleSize.height - 50));
    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);

    std::string filepath = FileUtils::getInstance()->fullPathForFilename("plot/plot_3.txt");
    std::string content = FileUtils::getInstance()->getStringFromFile(filepath);
    auto plot = Label::createWithTTF("","fonts/manual_font/SourceHanSerifCN-Bold-2.otf",30);
    plot->setString(content);
    plot->setColor(Color3B::WHITE);
    plot->setPosition(Vec2(origin.x + visibleSize.width / 2,origin.y + visibleSize.height / 2));
    this->addChild(plot,1);
    return true;
}