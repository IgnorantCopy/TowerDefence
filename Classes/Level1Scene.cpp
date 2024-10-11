#include "Level1Scene.h"
#include "SelectLevelScene.h"

USING_NS_CC;

Scene* Level1Scene::createScene()
{
    return Level1Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level1Scene.cpp\n");
}

bool Level1Scene::init()
{
    if ( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("images/level1_background.png",Rect(0,0,2500,1500));
    if(background == nullptr) {
        problemLoading("'images/level1_background.png'");
    } else {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        this->addChild(background, 0);
    }
    
    auto frameBase = Sprite::create("images/frame_base.png");
    if(frameBase == nullptr) {
        problemLoading("'images/frame_base.png'");
    } else {
        frameBase->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 1500 - visibleSize.height));
        this->addChild(frameBase, 1);
    }
    
    float gap = 300;
    this->selectedTower = Sprite::create("images/towers/archer_base_onblock.png");
    this->selectedTower->setVisible(false);
    this->selectedTower->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(this->selectedTower, 5);
    this->archerBaseSelector = ui::Button::create(
            "images/towers/archer_base.png",
            "images/towers/archer_base.png",
            "images/towers/archer_base_inactive.png"
    );
    this->archerBaseSelector->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                this->isSelecting = 1;
                this->selectedTower->setTexture("images/towers/archer_base_onblock.png");
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });
    this->archerBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 - 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    this->addChild(this->archerBaseSelector, 4);
    
    this->magicianBaseSelector = ui::Button::create(
            "images/towers/magician_base.png",
            "images/towers/magician_base.png",
            "images/towers/magician_base_inactive.png"
    );
    this->magicianBaseSelector->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                this->isSelecting = 2;
                this->selectedTower->setTexture("images/towers/magician_base_onblock.png");
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });
    this->magicianBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                           origin.y + 1680 - visibleSize.height));
    this->addChild(this->magicianBaseSelector, 4);
    
    this->helperBaseSelector = ui::Button::create(
            "images/towers/helper_base.png",
            "images/towers/helper_base.png",
            "images/towers/helper_base_inactive.png"
    );
    this->helperBaseSelector->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                this->isSelecting = 3;
                this->selectedTower->setTexture("images/towers/helper_base_onblock.png");
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            default:
                break;
        }
    });
    this->helperBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 + 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    this->addChild(this->helperBaseSelector, 4);
    
    // tower info
    this->deleteButton = ui::Button::create(
            "images/delete.png",
            "images/delete.png",
            "images/delete.png"
    );
    this->deleteButton->setVisible(false);
    this->deleteButton->setSwallowTouches(true);
    this->deleteButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->deleteTower();
                break;
            default:
                break;
        }
    });
    this->upgradeButton = ui::Button::create(
            "images/upgrade.png",
            "images/upgrade.png",
            "images/upgrade.png"
    );
    this->upgradeButton->setVisible(false);
    this->upgradeButton->setSwallowTouches(true);
    this->upgradeButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->upgradeTower();
                break;
            default:
                break;
        }
    });
    this->towerInfoButton = ui::Button::create(
            "images/info.png",
            "images/info.png",
            "images/info.png"
    );
    this->towerInfoButton->setVisible(false);
    this->towerInfoButton->setSwallowTouches(true);
    this->towerInfoButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->showTowerInfo();
                break;
            default:
                break;
        }
    });
    this->skillButton = ui::Button::create(
            "images/towers/skill_icon/archer_base.png",
            "images/towers/skill_icon/archer_base.png",
            "images/towers/skill_icon/archer_base_inactive.png"
    );
    this->skillButton->setVisible(false);
    this->skillButton->setSwallowTouches(true);
    this->skillButton->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType type) {
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->executeSkill();
                break;
            default:
                break;
        }
    });
    this->addChild(this->deleteButton, 4);
    this->addChild(this->upgradeButton, 4);
    this->addChild(this->towerInfoButton, 4);
    this->addChild(this->skillButton, 4);
    
    // the back button to go back to the SelectLevel scene
    auto Back=Label::createWithTTF("Back", "fonts/Bender/BENDER.OTF", 75);
    auto backItem=MenuItemLabel::create(
            Back,
            [this](Ref *ref){
                Director::getInstance()->replaceScene(TransitionCrossFade::create(0.4f, SelectLevelScene::createScene()));
            }
    );
    backItem->setPosition(Vec2(origin.x + visibleSize.width - 100,
                               origin.y + visibleSize.height - 50));

    //create map
    float delta = 140;
    float x = origin.x + 350 + delta;
    float y = origin.y + visibleSize.height - delta;
    createMap(1);
    for(size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            Grid::Type type_ = map->grids[map->shape.index_of(i, j)].type;
            if(type_ != Grid::Type::None) {
                grid[i][j] = ui::Button::create(images[type_], images[type_]);
                grid[i][j]->setPosition(Vec2(x + j * SIZE, y - i * SIZE));
                this->addChild(grid[i][j], 2);
            }
        }
    }
    
    auto blockBackground = Sprite::create("images/block_background.png", Rect(0, 0, 1680, 980));
    if(blockBackground == nullptr) {
        problemLoading("'images/block_background.png'");
    } else {
        blockBackground->setPosition(Vec2(x + 5.5f * delta, y - 3 * delta));
        this->addChild(blockBackground, 1);
    }
    
    auto money = Sprite::create("images/gold.png");
    if(money == nullptr) {
        problemLoading("'images/gold.png'");
    } else {
        money->setPosition(Vec2(origin.x + 70, origin.y + visibleSize.height - 70));
        this->addChild(money, 1);
    }
    this->moneyLabel = Label::createWithTTF("0", "fonts/Bender/BENDER.OTF", 75);
    this->moneyLabel->setPosition(Vec2(origin.x + 150, origin.y + visibleSize.height - 70));
    this->addChild(this->moneyLabel, 1);
    
    auto enemyExample = Sprite::create("images/enemies/dog/move/dog_move00.png");
    if (enemyExample == nullptr) {
        problemLoading("'images/enemies/dog/move/dog_move00.png'");
    } else {
        enemyExample->setPosition(Vec2(origin.x, origin.y + visibleSize.height / 2));
        enemyExample->setScale(0.25f);
        this->addChild(enemyExample, 5);
        Vector<SpriteFrame*> frames;
        int num = 15;
        frames.reserve(num);
        for (int i = 0; i < num; i++) {
            std::string path = "images/enemies/dog/move/dog_move";
            path += (i < 10 ? "0" : "") + std::to_string(i) + ".png";
            frames.pushBack(SpriteFrame::create(path, Rect(0, 0, 900, 900)));
        }
        Animation* animation = Animation::createWithSpriteFrames(frames, 0.05f);
        Animate* animate = Animate::create(animation);
        
        auto moveBy = MoveBy::create(0.8f, Vec2(140, 0));
        auto spawn = Spawn::create(moveBy, animate, nullptr);
        enemyExample->runAction(RepeatForever::create(spawn));
    }
    
    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);
    
    // add a mouse click event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Level1Scene::onMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(Level1Scene::onMouseMove, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(Level1Scene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}