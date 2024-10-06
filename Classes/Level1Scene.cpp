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
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Level1Scene::init()
{
    if ( !Scene::init() ){
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(Level1Scene::menuCloseCallback, this));
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

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
    Grid::grid_type type[7][12]={Grid::grid_type::block_path};
    type[0][0]=type[0][11]=type[2][11]=type[3][11]=type[4][11]=type[6][0]=Grid::grid_type::block_out;
    type[2][0]=type[3][0]=type[4][0]=type[6][11]=Grid::grid_type::block_in;
    type[0][7]=type[6][7]=Grid::grid_type::block_transport;
    type[0][5]=type[0][6]=type[1][0]=type[1][8]=type[1][9]=type[1][10]=type[1][11]=type[5][0]=type[5][7]=type[5][11]=type[6][6]=Grid::grid_type::none;
    type[1][1]=type[1][2]=type[1][3]=type[1][5]=type[1][6]=type[1][7]=type[3][2]=type[4][6]=type[4][7]=type[5][1]=type[5][2]=
    type[5][3]=type[5][4]=type[5][6]=type[5][8]=type[5][9]=type[5][10]=Grid::grid_type::block_tower;
    std::vector<std::string> grid_image = { "images/block_low.png", "images/in.png", "images/out.png", "images/block_transport.png", "images/block_high.png" };

    towerdefence::core::Map map(12,7,[&](size_t x, size_t y) -> Grid{ return Grid(type[x][y]); });
    for(size_t i = 0; i < map.shape.height_; i++) {
        for (size_t j = 0; j < map.shape.width_; j++) {
            if(type[i][j] != Grid::grid_type::none) {
                Grid::grid_type type_ = map.grids[map.shape.index_of(i,j)].type;
                grid[i][j] = ui::Button::create(grid_image[type_], grid_image[type_]);
                grid[i][j]->setPosition(Vec2(x + j * SIZE, y - i * SIZE));
                this->addChild(grid[i][j], 1);
            }
        }
    }

    Vector<MenuItem*> MenuItems;
    MenuItems.pushBack(backItem);
    auto menu = Menu::createWithArray(MenuItems);
    this->addChild(menu, MenuItems.size());
    menu->setPosition(Vec2::ZERO);

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