#include "Level2Scene.h"
#include "ui/CocosGUI.h"
#include "SelectLevelScene.h"

USING_NS_CC;
using towerdefence::core::Grid;
using towerdefence::core::Map;

Scene* Level2Scene::createScene()
{
    return Level2Scene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level2Scene.cpp\n");
}

bool Level2Scene::init()
{
    if ( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("images/level2_background.png",Rect(0,0,2500,1500));
    if(background == nullptr) {
        problemLoading("'images/level2_background.png'");
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
    auto archerBaseSelector = Sprite::create("images/towers/archer_base.png");
    if(archerBaseSelector == nullptr) {
        problemLoading("'images/towers/archer_base.png'");
    } else {
        archerBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 - 2 * gap,
                                             origin.y + 1680 - visibleSize.height));
        this->addChild(archerBaseSelector, 2);
    }

    auto magicianBaseSelector = Sprite::create("images/towers/magician_base.png");
    if(magicianBaseSelector == nullptr) {
        problemLoading("'images/towers/magician_base.png'");
    } else {
        magicianBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                               origin.y + 1680 - visibleSize.height));
        this->addChild(magicianBaseSelector, 2);
    }

    auto helperBaseSelector = Sprite::create("images/towers/helper_base.png");
    if(helperBaseSelector == nullptr) {
        problemLoading("'images/towers/helper_base.png'");
    } else {
        helperBaseSelector->setPosition(Vec2(origin.x + visibleSize.width / 2 + 2 * gap,
                                             origin.y + 1680 - visibleSize.height));
        this->addChild(helperBaseSelector, 2);
    }

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
    type[2][11]=type[0][11]=type[3][11]=type[4][11]=type[6][11]=Grid::Type::BlockOut;
    type[2][1]=type[3][1]=type[6][1]=Grid::Type::BlockIn;
    type[0][0]=type[0][8]=type[6][8]=Grid::Type::BlockTransport;
    type[0][1]=type[0][7]=type[1][1]=type[1][8]=type[1][9]=type[1][10]=
    type[1][11]=type[5][1]=type[5][8]=type[5][11]=type[5][9]=type[5][10]=Grid::Type::None;
    type[0][2]=type[0][3]=type[0][4]=type[0][5]=type[0][6]=type[1][2]=type[1][6]=type[1][7]=
    type[3][4]=type[3][6]=type[4][2]=type[4][1]=type[6][3]=type[6][4]=type[6][5]=type[6][6]=type[6][7]=Grid::Type::BlockTower;
    std::vector<std::string> images = { "images/block_low.png", "images/in.png", "images/out.png", "images/block_transport.png", "images/block_high.png" };

    for(size_t i = 0; i < 7; i++) {
        for (size_t j = 0; j < 12; j++) {
            if(type[i][j] != Grid::Type::None) {
                if( i==0 && j==0 ) grid[i][j] = ui::Button::create("images/block_transport_D.png", "images/block_transport_D.png");
                else grid[i][j] = ui::Button::create(images[type[i][j]], images[type[i][j]]);
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

    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);
    
    // add a mouse click event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Level2Scene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    return true;
}

void Level2Scene::menuCloseCallback(cocos2d::Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void Level2Scene::onMouseDown(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    
    auto particle = ParticleSystemQuad::create("particles/mouse.plist");
    if (particle == nullptr) {
        problemLoading("'particles/mouse.plist'");
    } else {
        particle->setPosition(Vec2(x, y));
        this->addChild(particle, 3);
    }
}