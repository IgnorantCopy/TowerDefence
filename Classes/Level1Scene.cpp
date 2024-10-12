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
    auto archerBaseSelector = MenuItemImage::create(
            "images/towers/archer_base.png",
            "images/towers/archer_base.png",
            [this](Ref *ref) {
                this->isSelecting = 1;
                this->selectedTower->setTexture("images/towers/archer_base_onblock.png");
            }
    );
    archerBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 - 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    
    auto magicianBaseSelector = MenuItemImage::create(
            "images/towers/magician_base.png",
            "images/towers/magician_base.png",
            [this](Ref *ref) {
                this->isSelecting = 2;
                this->selectedTower->setTexture("images/towers/magician_base_onblock.png");
            }
    );
    magicianBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                           origin.y + 1680 - visibleSize.height));
    
    auto helperBaseSelector = MenuItemImage::create(
            "images/towers/helper_base.png",
            "images/towers/helper_base.png",
            [this](Ref *ref) {
                this->isSelecting = 3;
                this->selectedTower->setTexture("images/towers/helper_base_onblock.png");
            }
    );
    helperBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 + 2 * gap,
                                         origin.y + 1680 - visibleSize.height));



//    schedule([this](float dt) {
//        this->update();
//    }, 0.1f, "update");

    // tower info
    this->deleteItem = MenuItemImage::create(
            "images/delete.png",
            "images/delete.png",
            [this](Ref *ref) {
                scheduleOnce([this](float dt) {
                    this->deleteTower();
                }, 0.2f, "deleteTower");
            }
    );
    this->deleteItem->setVisible(false);
    this->upgradeItem = MenuItemImage::create(
            "images/upgrade.png",
            "images/upgrade.png",
            [this](Ref *ref) {
                // TODO: upgrade the tower
                log("upgrade");
            }
    );
    this->upgradeItem->setVisible(false);
    this->towerInfoItem = MenuItemImage::create(
            "images/info.png",
            "images/info.png",
            [this](Ref *ref) {
                // TODO: show the tower info
                log("info");
            }
    );
    this->towerInfoItem->setVisible(false);
    this->skillItem = MenuItemImage::create(
            "images/towers/skill_icon/archer_base.png",
            "images/towers/skill_icon/archer_base.png",
            [this](Ref *ref) {
                // TODO: execute the skill
                log("skill");
            }
    );
    this->skillItem->setVisible(false);
    
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
    moneyLabel = cocos2d::Label::createWithTTF(std::to_string(map->getcost_()), "fonts/Bender/BENDER.OTF", 75);
    this->moneyLabel->setPosition(cocos2d::Vec2(origin.x + 150 + 15 * log10(map->getcost_()), origin.y + visibleSize.height - 70));
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
    MenuItems.pushBack(archerBaseSelector);
    MenuItems.pushBack(magicianBaseSelector);
    MenuItems.pushBack(helperBaseSelector);
    MenuItems.pushBack(this->deleteItem);
    MenuItems.pushBack(this->upgradeItem);
    MenuItems.pushBack(this->towerInfoItem);
    MenuItems.pushBack(this->skillItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, 4);
    menu->setPosition(Vec2::ZERO);
    
    // add a mouse click event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Level1Scene::onMouseDown, this);
    mouseListener->onMouseMove = CC_CALLBACK_1(Level1Scene::onMouseMove, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(Level1Scene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // update the map
    schedule([this](float dt){
        map->update();
        this->update();
    }, 1.0/30, "update");

    return true;
}