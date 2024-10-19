#include "manual.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "core/entity/entity.h"

USING_NS_CC;

Scene *manual::createScene() {
    return manual::create();
}

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool manual::init() {
    if (!Scene::init())
        return false;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto userDefault = UserDefault::getInstance();

    auto background = Sprite::create("images/manual_background.png", Rect(0, 0, 2500, 1500));
    if (background == nullptr) {
        problemLoading("'images/manual_background.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 1);
    }

    auto Back = Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem = MenuItemLabel::create(
            Back,
            [this](Ref *ref) {
                Director::getInstance()->popSceneWithTransition<TransitionCrossFade>(0.5f);
            }
    );
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 50));

    auto Clear = Label::createWithTTF("Clear Save Data", "fonts/Bender/BENDER.OTF", 75);
    auto clearItem = MenuItemLabel::create(
            Clear,
            [this](Ref *ref) {
                auto userDefault = UserDefault::getInstance();
                userDefault->setIntegerForKey("level1", 0);
                userDefault->setIntegerForKey("level2", 4);
                userDefault->setIntegerForKey("level3", 4);
        }
    );
    clearItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 275));
    clearItem->setVisible(false);

    auto Setting = Label::createWithTTF("Setting", "fonts/Bender/BENDER.OTF", 75);
    auto settingItem = MenuItemLabel::create(
        Setting,
        [this, clearItem, userDefault](Ref *ref) {
            if (userDefault->getBoolForKey("clearItemShow", true) && !clearItem->isVisible()) {
                clearItem->setVisible(true);
                clearItem->setOpacity(0);
                auto fadein = FadeIn::create(1.0f);
                clearItem->runAction(fadein);
            }
        }
    );
    settingItem->setPosition(Vec2(origin.x + 150, origin.y + visibleSize.height - 50));

    Vector<MenuItem *> MenuItems;
    MenuItems.pushBack(backItem);
    MenuItems.pushBack(clearItem);
    MenuItems.pushBack(settingItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);

    std::string content_archer_base, content_highspeed_archer, content_bomber, content_archer,
            content_magician_base, content_core_magician, content_diffusive_magician, content_special_magician,
            content_helper_base, content_decelerate_magician, content_weaken_magician, content_aggressive_magician;
    std::string filepath_archer_base, filepath_highspeed_archer, filepath_bomber, filepath_archer,
            filepath_magician_base, filepath_core_magician, filepath_diffusive_magician, filepath_special_magician,
            filepath_helper_base, filepath_decelerate_magician, filepath_weaken_magician, filepath_aggressive_magician;
    filepath_archer_base = FileUtils::getInstance()->fullPathForFilename("manual/archer_base.txt");
    content_archer_base = FileUtils::getInstance()->getStringFromFile(filepath_archer_base);
    filepath_highspeed_archer = FileUtils::getInstance()->fullPathForFilename("manual/highspeed_archer.txt");
    content_highspeed_archer = FileUtils::getInstance()->getStringFromFile(filepath_highspeed_archer);
    filepath_bomber = FileUtils::getInstance()->fullPathForFilename("manual/bomber.txt");
    content_bomber = FileUtils::getInstance()->getStringFromFile(filepath_bomber);
    filepath_archer = FileUtils::getInstance()->fullPathForFilename("manual/archer.txt");
    content_archer = FileUtils::getInstance()->getStringFromFile(filepath_archer);
    filepath_magician_base = FileUtils::getInstance()->fullPathForFilename("manual/magician_base.txt");
    content_magician_base = FileUtils::getInstance()->getStringFromFile(filepath_magician_base);
    filepath_core_magician = FileUtils::getInstance()->fullPathForFilename("manual/core_magician.txt");
    content_core_magician = FileUtils::getInstance()->getStringFromFile(filepath_core_magician);
    filepath_diffusive_magician = FileUtils::getInstance()->fullPathForFilename("manual/diffusive_magician.txt");
    content_diffusive_magician = FileUtils::getInstance()->getStringFromFile(filepath_diffusive_magician);
    filepath_special_magician = FileUtils::getInstance()->fullPathForFilename("manual/special_magician.txt");
    content_special_magician = FileUtils::getInstance()->getStringFromFile(filepath_special_magician);
    filepath_helper_base = FileUtils::getInstance()->fullPathForFilename("manual/helper_base.txt");
    content_helper_base = FileUtils::getInstance()->getStringFromFile(filepath_helper_base);
    filepath_decelerate_magician = FileUtils::getInstance()->fullPathForFilename("manual/decelerate_magician.txt");
    content_decelerate_magician = FileUtils::getInstance()->getStringFromFile(filepath_decelerate_magician);
    filepath_weaken_magician = FileUtils::getInstance()->fullPathForFilename("manual/weaken_magician.txt");
    content_weaken_magician = FileUtils::getInstance()->getStringFromFile(filepath_weaken_magician);
    filepath_aggressive_magician = FileUtils::getInstance()->fullPathForFilename("manual/aggressive_magician.txt");
    content_aggressive_magician = FileUtils::getInstance()->getStringFromFile(filepath_aggressive_magician);

    auto description = Label::createWithTTF("", "fonts/manual_font/SourceHanSerifCN-Bold-2.otf", 30);;
    description->setString(content_archer_base);
    description->setColor(Color3B::YELLOW);
    description->setPosition(Vec2(origin.x + 2000, origin.y + 520));
    this->addChild(description, 1);

    auto button_ArcherBase = ui::Button::create("images/manual/archer_base.png");
    button_ArcherBase->addClickEventListener([content_archer_base, description](Ref *sender) {
        description->setString(content_archer_base);
    });
    button_ArcherBase->setPosition(Vec2(origin.x + 200, origin.y + 800));

    auto button_HighspeedArcher = ui::Button::create("images/manual/highspeed_archer.png");
    button_HighspeedArcher->addClickEventListener([content_highspeed_archer, description](Ref *sender) {
        description->setString(content_highspeed_archer);
    });
    button_HighspeedArcher->setPosition(Vec2(origin.x + 500, origin.y + 800));

    auto button_Bomber = ui::Button::create("images/manual/bomber.png");
    button_Bomber->addClickEventListener([content_bomber, description](Ref *sender) {
        description->setString(content_bomber);
    });
    button_Bomber->setPosition(Vec2(origin.x + 800, origin.y + 800));

    auto button_Archer = ui::Button::create("images/manual/archer.png");
    button_Archer->addClickEventListener([content_archer, description](Ref *sender) {
        description->setString(content_archer);
    });
    button_Archer->setPosition(Vec2(origin.x + 1100, origin.y + 800));

    auto button_MagicianBase = ui::Button::create("images/manual/magician_base.png");
    button_MagicianBase->addClickEventListener([content_magician_base, description](Ref *sender) {
        description->setString(content_magician_base);
    });
    button_MagicianBase->setPosition(Vec2(origin.x + 200, origin.y + 500));

    auto button_CoreMagician = ui::Button::create("images/manual/core_magician.png");
    button_CoreMagician->addClickEventListener([content_core_magician, description](Ref *sender) {
        description->setString(content_core_magician);
    });
    button_CoreMagician->setPosition(Vec2(origin.x + 500, origin.y + 500));

    auto button_DiffusiveMagician = ui::Button::create("images/manual/diffusive_magician.png");
    button_DiffusiveMagician->addClickEventListener([content_diffusive_magician, description](Ref *sender) {
        description->setString(content_diffusive_magician);
    });
    button_DiffusiveMagician->setPosition(Vec2(origin.x + 800, origin.y + 500));

    auto button_SpecialMagician = ui::Button::create("images/manual/special_magician.png");
    button_SpecialMagician->addClickEventListener([content_special_magician, description](Ref *sender) {
        description->setString(content_special_magician);
    });
    button_SpecialMagician->setPosition(Vec2(origin.x + 1100, origin.y + 500));

    auto button_HelperBase = ui::Button::create("images/manual/helper_base.png");
    button_HelperBase->addClickEventListener([content_helper_base, description](Ref *sender) {
        description->setString(content_helper_base);
    });
    button_HelperBase->setPosition(Vec2(origin.x + 200, origin.y + 200));

    auto button_DecelerateMagician = ui::Button::create("images/manual/decelerate_magician.png");
    button_DecelerateMagician->addClickEventListener([content_decelerate_magician, description](Ref *sender) {
        description->setString(content_decelerate_magician);
    });
    button_DecelerateMagician->setPosition(Vec2(origin.x + 500, origin.y + 200));

    auto button_WeakenMagician = ui::Button::create("images/manual/weaken_magician.png");
    button_WeakenMagician->addClickEventListener([content_weaken_magician, description](Ref *sender) {
        description->setString(content_weaken_magician);
    });
    button_WeakenMagician->setPosition(Vec2(origin.x + 800, origin.y + 200));

    auto button_AggressiveMagician = ui::Button::create("images/manual/aggressive_magician.png");
    button_AggressiveMagician->addClickEventListener([content_aggressive_magician, description](Ref *sender) {
        description->setString(content_aggressive_magician);
    });
    button_AggressiveMagician->setPosition(Vec2(origin.x + 1100, origin.y + 200));

    this->addChild(button_ArcherBase, 1);
    this->addChild(button_HighspeedArcher, 1);
    this->addChild(button_Bomber, 1);
    this->addChild(button_Archer, 1);
    this->addChild(button_MagicianBase, 1);
    this->addChild(button_CoreMagician, 1);
    this->addChild(button_DiffusiveMagician, 1);
    this->addChild(button_SpecialMagician, 1);
    this->addChild(button_HelperBase, 1);
    this->addChild(button_DecelerateMagician, 1);
    this->addChild(button_WeakenMagician, 1);
    this->addChild(button_AggressiveMagician, 1);

    return true;
}