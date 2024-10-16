#include "manual.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "core/entity/entity.h"

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
                Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4f,HelloWorld::createScene()));
            }
    );
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,origin.y + visibleSize.height - 50));

    std::string content;
    auto description=Label::createWithTTF("","Microsoft YaHei",70);
    content=FileUtils::getStringFromFile("Resources/manual/archer_base.txt");
    description->setString(content);
    this->addChild(description);

    auto button_ArcherBase=ui::Button::create("images/towers/archer_base.png");
    button_ArcherBase->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                    break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/archer_base.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_ArcherBase->setPosition(Vec2(origin.x + 200,origin.y + 200));

    auto button_HighspeedArcher=ui::Button::create("images/towers/highspeed_archer.png");
    button_HighspeedArcher->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/highspeed_archer.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_HighspeedArcher->setPosition(Vec2(origin.x + 500,origin.y + 200));

    auto button_Bomber=ui::Button::create("images/towers/bomber.png");
    button_HighspeedArcher->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/bomber.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_Bomber->setPosition(Vec2(origin.x + 800,origin.y + 200));

    auto button_Archer=ui::Button::create("images/towers/archer.png");
    button_Archer->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/archer.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_Archer->setPosition(Vec2(origin.x + 1100,origin.y + 200));

    auto button_MagicianBase=ui::Button::create("images/towers/magician_base.png");
    button_MagicianBase->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/magician_base.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_MagicianBase->setPosition(Vec2(origin.x + 200,origin.y + 500));

    auto button_CoreMagician=ui::Button::create("images/towers/core_magician.png");
    button_CoreMagician->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/core_magician.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_CoreMagician->setPosition(Vec2(origin.x + 500,origin.y + 500));

    auto button_DiffusiveMagician=ui::Button::create("images/towers/diffusive_magician.png");
    button_DiffusiveMagician->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/diffusive_magician.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_DiffusiveMagician->setPosition(Vec2(origin.x + 800,origin.y + 500));

    auto button_SpecialMagician=ui::Button::create("images/towers/special_magician.png");
    button_SpecialMagician->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/special_magician.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_SpecialMagician->setPosition(Vec2(origin.x + 1100,origin.y + 500));

    auto button_HelperBase=ui::Button::create("images/towers/helper_base.png");
    button_HelperBase->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/helper_base.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_HelperBase->setPosition(Vec2(origin.x + 200,origin.y + 800));

    auto button_DecelerateMagician=ui::Button::create("images/towers/decelerate_magician.png");
    button_DecelerateMagician->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/decelerate_magician.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_DecelerateMagician->setPosition(Vec2(origin.x + 500,origin.y + 800));

    auto button_WeakenMagician=ui::Button::create("images/towers/weaken_magician.png");
    button_WeakenMagician->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/weaken_magician.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_WeakenMagician->setPosition(Vec2(origin.x + 800,origin.y + 800));

    auto button_AggressiveMagician=ui::Button::create("images/towers/aggressive_magician.png");
    button_AggressiveMagician->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                content=FileUtils::getStringFromFile("Resources/manual/aggressive_magician.txt");
                description->setString(content);
                break;
            default:
                break;
        }
    });
    button_AggressiveMagician->setPosition(Vec2(origin.x + 1100,origin.y + 800));

    this->addChild(backItem);
    this->addChild(button_ArcherBase);
    this->addChild(button_HighspeedArcher);
    this->addChild(button_Bomber);
    this->addChild(button_Archer);
    this->addChild(button_MagicianBase);
    this->addChild(button_CoreMagician);
    this->addChild(button_DiffusiveMagician);
    this->addChild(button_SpecialMagician);
    this->addChild(button_HelperBase);
    this->addChild(button_DecelerateMagician);
    this->addChild(button_WeakenMagician);
    this->addChild(button_AggressiveMagician);

    return true;
}