#include "LevelScene.h"
#include "ui/CocosGUI.h"
#include "core/entity/tower/archer_base.h"
#include "core/entity/tower/magician_base.h"
#include "core/entity/tower/helper_base.h"


USING_NS_CC;
using towerdefence::core::Tower;
using towerdefence::core::ArcherBase;
using towerdefence::core::MagicianBase;
using towerdefence::core::HelperBase;

static void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level1Scene.cpp\n");
}

void LevelScene::cancelSelect() {
    this->selectedTower->setOpacity(0);
    this->isSelecting = 0;
}

void LevelScene::menuCloseCallback(cocos2d::Ref *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);

}

void LevelScene::createMap(int level)
{
    switch(level){
        case 1:
            type[0][0]=type[0][11]=type[2][11]=type[3][11]=type[4][11]=type[6][0]=Grid::Type::BlockOut;
            type[2][0]=type[3][0]=type[4][0]=type[6][11]=Grid::Type::BlockIn;
            type[0][7]=type[6][7]=Grid::Type::BlockTransport;
            type[0][5]=type[0][6]=type[1][0]=type[1][8]=type[1][9]=type[1][10]=
            type[1][11]=type[5][0]=type[5][7]=type[5][11]=type[6][6]=Grid::Type::None;
            type[1][1]=type[1][2]=type[1][3]=type[1][5]=type[1][6]=type[1][7]=type[3][2]=type[4][6]=type[4][7]=type[5][1]=type[5][2]=
            type[5][3]=type[5][4]=type[5][6]=type[5][8]=type[5][9]=type[5][10]=Grid::Type::BlockTower;
            map = new towerdefence::core::Map(width, height, [&](size_t x,size_t y) -> Grid{ return Grid(type[x][y]);});
            break;
        case 2:
            type[2][11]=type[0][11]=type[3][11]=type[4][11]=type[6][11]=Grid::Type::BlockOut;
            type[2][1]=type[3][1]=type[6][1]=Grid::Type::BlockIn;
            type[0][0]=type[0][8]=type[6][8]=Grid::Type::BlockTransport;
            type[0][1]=type[0][7]=type[1][1]=type[1][8]=type[1][9]=type[1][10]=
            type[1][11]=type[5][1]=type[5][8]=type[5][11]=type[5][9]=type[5][10]=Grid::Type::None;
            type[0][2]=type[0][3]=type[0][4]=type[0][5]=type[0][6]=type[1][2]=type[1][6]=type[1][7]=
            type[3][4]=type[3][6]=type[4][2]=type[4][1]=type[6][3]=type[6][4]=type[6][5]=type[6][6]=type[6][7]=Grid::Type::BlockTower;
            map = new towerdefence::core::Map(width, height, [&](size_t x,size_t y) -> Grid{ return Grid(type[x][y]);});
            break;
        case 3:
            type[0][0]=type[0][5]=type[0][6]=type[1][11]=type[2][0]=Grid::Type::BlockOut;
            type[3][11]=type[4][11]=type[6][0]=Grid::Type::BlockIn;
            type[0][4]=type[3][1]=type[6][7]=type[6][11]=Grid::Type::BlockTransport;
            type[0][8]=type[0][7]=type[0][9]=type[0][10]=type[0][11]=type[1][0]=
            type[1][1]=type[1][2]=type[3][0]=type[4][0]=type[5][0]=type[5][4]=type[5][5]=
            type[5][6]=type[5][10]=type[5][11]=type[6][3]=type[6][4]=type[6][5]=type[6][6]=Grid::Type::None;
            type[2][7]=type[1][3]=type[1][4]=type[2][8]=type[2][9]=type[2][10]=type[2][11]=type[4][1]=
            type[4][3]=type[4][4]=type[5][1]=type[5][3]=type[5][7]=type[5][8]=type[5][9]=Grid::Type::BlockTower;
            map = new towerdefence::core::Map(width, height, [&](size_t x,size_t y) -> Grid{ return Grid(type[x][y]);});
            break;
        default:
            break;
    }
}

void LevelScene::onMouseDown(cocos2d::Event *event) {
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
    
    if (this->isSelecting && this->selectedTower) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float delta = 140;
        float typeX = origin.x + 350 + delta;
        float typeY = origin.y + visibleSize.height - delta;
        if (x >= typeX - 0.5f * delta && x <= typeX + 11.5f * delta &&
            y >= typeY - 6.5f * delta && y <= typeY + 0.5f * delta) {
            int indexX = (int) ((x - typeX + 0.5f * delta) / delta);
            int indexY = (int) ((typeY - y + 0.5f * delta) / delta);
            if (this->type[indexY][indexX] == Grid::Type::BlockTower) {
                if (!this->map->get_ref(indexY, indexX).grid.tower.has_value()) {
                    std::string path = "images/towers/";
                    Tower *newTower;
                    Id newId = this->map->assign_id();
                    switch (this->isSelecting) {
                        case 1:
                            path += "archer_base_onblock.png";
                            newTower = new ArcherBase(newId, this->map->get_ref(indexY, indexX).clock());
                            break;
                        case 2:
                            path += "magician_base_onblock.png";
                            newTower = new MagicianBase(newId, this->map->get_ref(indexY, indexX).clock());
                            break;
                        case 3:
                            path += "helper_base_onblock.png";
                            newTower = new HelperBase(newId, this->map->get_ref(indexY, indexX).clock());
                            break;
                        default:
                            break;
                    }
                    this->map->get_ref(indexY, indexX).grid.tower.emplace(newTower);
                    auto newTowerSprite = Sprite::create(path);
                    newTowerSprite->setPosition(Vec2(typeX + indexX * delta, typeY - indexY * delta));
                    this->addChild(newTowerSprite, 3);
                    this->towers.push_back(std::make_pair(newId, newTowerSprite));
                }
                this->cancelSelect();
            }
        }
    }
}

void LevelScene::onMouseUp(cocos2d::Event *event) {
    EventMouse const* e = (EventMouse*)event;
    if (this->isSelecting && this->selectedTower && e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        this->cancelSelect();
    }
}

void LevelScene::onMouseMove(cocos2d::Event *event) {
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
            if (this->type[indexY][indexX] == Grid::Type::BlockTower) {
                this->selectedTower->setPosition(Vec2(typeX + indexX * delta, typeY - indexY * delta));
            } else {
                this->selectedTower->setPosition(Vec2(x, y));
            }
        } else {
            this->selectedTower->setPosition(Vec2(x, y));
        }
    }
}