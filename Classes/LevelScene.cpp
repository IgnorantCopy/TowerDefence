#include "LevelScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

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