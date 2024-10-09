#include "Level1Scene.h"
#include "ui/CocosGUI.h"
#include "SelectLevelScene.h"

USING_NS_CC;
using towerdefence::core::Grid;
using towerdefence::core::Map;

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
    this->selectedTower->setOpacity(0);
    this->selectedTower->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(this->selectedTower, 5);
    auto archerBaseSelector = MenuItemImage::create(
            "images/towers/archer_base.png",
            "images/towers/archer_base.png",
            [this](Ref *ref) {
                this->isSelecting = true;
                this->selectedTower->setTexture("images/towers/archer_base_onblock.png");
            }
    );
    archerBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 - 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    
    auto magicianBaseSelector = MenuItemImage::create(
            "images/towers/magician_base.png",
            "images/towers/magician_base.png",
            [this](Ref *ref) {
                this->isSelecting = true;
                this->selectedTower->setTexture("images/towers/magician_base_onblock.png");
            }
    );
    magicianBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                           origin.y + 1680 - visibleSize.height));
    
    auto helperBaseSelector = MenuItemImage::create(
            "images/towers/helper_base.png",
            "images/towers/helper_base.png",
            [this](Ref *ref) {
                this->isSelecting = true;
                this->selectedTower->setTexture("images/towers/helper_base_onblock.png");
            }
    );
    helperBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 + 2 * gap,
                                         origin.y + 1680 - visibleSize.height));
    
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
    float SIZE = 140.0;
    ui::Button* grid[7][12]={};
    Grid::Type type[7][12]={ Grid::Type::BlockPath };
    type[0][0]=type[0][11]=type[2][11]=type[3][11]=type[4][11]=type[6][0]=Grid::Type::BlockOut;
    type[2][0]=type[3][0]=type[4][0]=type[6][11]=Grid::Type::BlockIn;
    type[0][7]=type[6][7]=Grid::Type::BlockTransport;
    type[0][5]=type[0][6]=type[1][0]=type[1][8]=type[1][9]=type[1][10]=
    type[1][11]=type[5][0]=type[5][7]=type[5][11]=type[6][6]=Grid::Type::None;
    type[1][1]=type[1][2]=type[1][3]=type[1][5]=type[1][6]=type[1][7]=type[3][2]=type[4][6]=type[4][7]=type[5][1]=type[5][2]=
    type[5][3]=type[5][4]=type[5][6]=type[5][8]=type[5][9]=type[5][10]=Grid::Type::BlockTower;
    std::vector<std::string> images = { "images/block_low.png", "images/in.png", "images/out.png",
                                        "images/block_transport.png", "images/block_high.png" };

    for(size_t i = 0; i < 7; i++) {
        for (size_t j = 0; j < 12; j++) {
            if(type[i][j] != Grid::Type::None) {
                grid[i][j] = ui::Button::create(images[type[i][j]], images[type[i][j]]);
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
        
        auto moveBy = MoveBy::create(0.75f, Vec2(140, 0));
        auto spawn = Spawn::create(moveBy, animate, nullptr);
        enemyExample->runAction(RepeatForever::create(spawn));
    }
    
    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    MenuItems.pushBack(archerBaseSelector);
    MenuItems.pushBack(magicianBaseSelector);
    MenuItems.pushBack(helperBaseSelector);
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

void Level1Scene::menuCloseCallback(cocos2d::Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void Level1Scene::onMouseDown(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    
    auto particle = ParticleSystemQuad::create("particles/mouse.plist");
    if (particle == nullptr) {
        problemLoading("'particles/mouse.plist'");
    } else {
        particle->setPosition(Vec2(x, y));
        this->addChild(particle, 5);
    }
}

void Level1Scene::onMouseUp(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    if (this->isSelecting && this->selectedTower && e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        this->selectedTower->setOpacity(0);
        this->isSelecting = false;
    }
}

void Level1Scene::onMouseMove(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    
    if (this->isSelecting && this->selectedTower) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float delta = 140;
        float typeX = origin.x + 350 + delta;
        float typeY = origin.y + visibleSize.height - delta;
        this->selectedTower->setOpacity(255);
        if (x >= typeX - 0.5f * delta && x <= typeX + 11.5f * delta &&
            y >= typeY - 6.5f * delta && y <= typeY + 0.5f * delta) {
            int indexX = (int)((x - typeX + 0.5f * delta) / delta);
            int indexY = (int)((typeY - y + 0.5f * delta) / delta);
            this->selectedTower->setPosition(Vec2(typeX + indexX * delta, typeY - indexY * delta));
        } else {
            this->selectedTower->setPosition(Vec2(x, y));
        }
    }
}