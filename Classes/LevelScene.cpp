#include "LevelScene.h"
#include "core/entity/entity.h"
#include "core/entity/tower/archer_base.h"
#include "core/entity/tower/helper_base.h"
#include "core/entity/tower/magician_base.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using towerdefence::core::ArcherBase;
using towerdefence::core::HelperBase;
using towerdefence::core::MagicianBase;

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in Level1Scene.cpp\n");
}

Sprite *LevelScene::getTower(Id id) {
    for (auto &pair : this->towers) {
        if (pair.first == id) {
            return pair.second;
        }
    }
}

Sprite *LevelScene::getEnemy(Id id) {
    for (auto &pair : this->enemies) {
        if (pair.first == id) {
            return pair.second;
        }
    }
}

void LevelScene::cancelSelect() {
    this->selectedTower->setVisible(false);
    this->isSelecting = 0;
}

void LevelScene::putTower(float x, float y) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + SIZE;
    float typeY = origin.y + visibleSize.height - SIZE;
    if (x >= typeX - 0.5f * SIZE && x <= typeX + 11.5f * SIZE &&
        y >= typeY - 6.5f * SIZE && y <= typeY + 0.5f * SIZE) {
        int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
        if (this->type[indexY][indexX] == Grid::Type::BlockTower) {
            if (!this->map->get_ref(indexY, indexX).grid.tower.has_value()) {
                std::string path = "images/towers/";
                towerdefence::core::TowerFactoryBase *newTower;
                switch (this->isSelecting) {
                case 1:
                    path += "archer_base_onblock.png";
                    newTower =
                        new towerdefence::core::TowerFactory<ArcherBase>{};
                    break;
                case 2:
                    path += "magician_base_onblock.png";
                    newTower =
                        new towerdefence::core::TowerFactory<MagicianBase>{};
                    break;
                case 3:
                    path += "helper_base_onblock.png";
                    newTower =
                        new towerdefence::core::TowerFactory<HelperBase>{};
                    break;
                default:
                    break;
                }
                auto id = this->map->spawn_tower_at(indexY, indexX, *newTower);
                auto newTowerSprite = Sprite::create(path);
                newTowerSprite->setPosition(
                    Vec2(typeX + indexX * SIZE, typeY - indexY * SIZE));
                this->addChild(newTowerSprite, 3);
                this->towers.push_back(std::make_pair(
                    id.value(),
                    newTowerSprite)); // todo: handle spawning failure
            }
            scheduleOnce([this](float dt) { this->cancelSelect(); }, 0.1f,
                         "cancelSelect");
        }
    }
}

void LevelScene::showTowerInfo(float x, float y) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + SIZE;
    float typeY = origin.y + visibleSize.height - SIZE;
    if (x >= typeX - 0.5f * SIZE && x <= typeX + 11.5f * SIZE &&
        y >= typeY - 6.5f * SIZE && y <= typeY + 0.5f * SIZE) {
        int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
        if (this->type[indexY][indexX] == Grid::Type::BlockTower &&
            this->map->get_ref(indexY, indexX).grid.tower.has_value()) {
            Id towerId =
                this->map->get_ref(indexY, indexX).grid.tower.value()->id;
            this->selectedTowerId = towerId;
            Sprite *towerSprite = this->getTower(towerId);
            if (towerSprite) {
                float towerX = towerSprite->getPositionX();
                float towerY = towerSprite->getPositionY();

                this->towerInfoItem->setPosition(Vec2(towerX, towerY));
                this->towerInfoItem->setScale(0.1f);
                this->towerInfoItem->setVisible(true);

                this->deleteItem->setPosition(Vec2(towerX, towerY));
                this->deleteItem->setScale(0.1f);
                this->deleteItem->setVisible(true);

                this->upgradeItem->setPosition(Vec2(towerX, towerY));
                this->upgradeItem->setScale(0.1f);
                this->upgradeItem->setVisible(true);

                this->skillItem->setPosition(Vec2(towerX, towerY));
                this->skillItem->setScale(0.1f);
                this->skillItem->setVisible(true);

                auto scale = ScaleBy::create(0.2f, 10);
                auto move1 = MoveBy::create(0.2f, Vec2(50, 90));
                auto move2 = MoveBy::create(0.2f, Vec2(-50, 90));
                auto move3 = MoveBy::create(0.2f, Vec2(50, -90));
                auto move4 = MoveBy::create(0.2f, Vec2(-50, -90));

                auto towerInfoSpawn = Spawn::create(scale, move3, nullptr);
                auto deleteSpawn =
                    Spawn::create(scale->clone(), move2, nullptr);
                auto upgradeSpawn =
                    Spawn::create(scale->clone(), move4, nullptr);
                auto skillSpawn = Spawn::create(scale->clone(), move1, nullptr);

                this->towerInfoItem->runAction(towerInfoSpawn);
                this->deleteItem->runAction(deleteSpawn);
                this->upgradeItem->runAction(upgradeSpawn);
                this->skillItem->runAction(skillSpawn);
                this->isShowingTowerInfo = true;
                scheduleOnce(
                    [this](float dt) { this->isShowingTowerInfo = false; },
                    0.2f, "preventHighSpeed");
            }
        }
    }
}

void LevelScene::hideTowerInfo(float x, float y) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + SIZE;
    float typeY = origin.y + visibleSize.height - SIZE;

    if (x >= typeX - 0.5f * SIZE && x <= typeX + 11.5f * SIZE &&
        y >= typeY - 6.5f * SIZE && y <= typeY + 0.5f * SIZE) {
        int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
        if (this->type[indexY][indexX] == Grid::Type::BlockTower &&
            this->map->get_ref(indexY, indexX).grid.tower.has_value()) {
            return;
        }
    }

    auto scale = ScaleBy::create(0.2f, 0.1f);
    auto move1 = MoveBy::create(0.2f, Vec2(-50, -90));
    auto move2 = MoveBy::create(0.2f, Vec2(50, -90));
    auto move3 = MoveBy::create(0.2f, Vec2(-50, 90));
    auto move4 = MoveBy::create(0.2f, Vec2(50, 90));

    auto towerInfoSpawn = Spawn::create(scale, move3, nullptr);
    auto deleteSpawn = Spawn::create(scale->clone(), move2, nullptr);
    auto upgradeSpawn = Spawn::create(scale->clone(), move4, nullptr);
    auto skillSpawn = Spawn::create(scale->clone(), move1, nullptr);

    this->towerInfoItem->runAction(towerInfoSpawn);
    this->deleteItem->runAction(deleteSpawn);
    this->upgradeItem->runAction(upgradeSpawn);
    this->skillItem->runAction(skillSpawn);
    this->isShowingTowerInfo = false;
    scheduleOnce(
        [this](float dt) {
            this->towerInfoItem->setVisible(false);
            this->deleteItem->setVisible(false);
            this->upgradeItem->setVisible(false);
            this->skillItem->setVisible(false);
        },
        0.2f, "hideTowerInfo");
}

void LevelScene::deleteTower() {
    Sprite *towerSprite = this->getTower(this->selectedTowerId);
    for (auto it = this->towers.begin(); it != this->towers.end(); ++it) {
        if (it->first == this->selectedTowerId) {
            this->towers.erase(it);
            break;
        }
    }
    if (towerSprite) {
        towerSprite->removeFromParent();
    }
       this->map->remove_tower(this->selectedTowerId);
}

void LevelScene::upgradeTower() {
    Sprite *towerSprite = this->getTower(this->selectedTowerId);
}

void LevelScene::menuCloseCallback(cocos2d::Ref *pSender) {
    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the
     * application  ,do not use Director::getInstance()->end() as given
     * above,instead trigger a custom event created in RootViewController.mm as
     * below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void LevelScene::createMap(int level) {
    switch (level) {
    case 1:
        type[0][0] = type[0][11] = type[2][11] = type[3][11] = type[4][11] =
            type[6][0] = Grid::Type::BlockOut;
        type[2][0] = type[3][0] = type[4][0] = type[6][11] =
            Grid::Type::BlockIn;
        type[0][7] = type[6][7] = Grid::Type::BlockTransport;
        type[0][5] = type[0][6] = type[1][0] = type[1][8] = type[1][9] =
            type[1][10] = type[1][11] = type[5][0] = type[5][7] = type[5][11] =
                type[6][6] = Grid::Type::None;
        type[1][1] = type[1][2] = type[1][3] = type[1][5] = type[1][6] =
            type[1][7] = type[3][2] = type[4][6] = type[4][7] = type[5][1] =
                type[5][2] = type[5][3] = type[5][4] = type[5][6] = type[5][8] =
                    type[5][9] = type[5][10] = Grid::Type::BlockTower;
        map = new towerdefence::core::Map(
            width, height,
            [&](size_t x, size_t y) -> Grid { return Grid(type[x][y]); });
        break;
    case 2:
        type[2][11] = type[0][11] = type[3][11] = type[4][11] = type[6][11] =
            Grid::Type::BlockOut;
        type[2][1] = type[3][1] = type[6][1] = Grid::Type::BlockIn;
        type[0][0] = type[0][8] = type[6][8] = Grid::Type::BlockTransport;
        type[0][1] = type[0][7] = type[1][1] = type[1][8] = type[1][9] =
            type[1][10] = type[1][11] = type[5][1] = type[5][8] = type[5][11] =
                type[5][9] = type[5][10] = Grid::Type::None;
        type[0][2] = type[0][3] = type[0][4] = type[0][5] = type[0][6] =
            type[1][2] = type[1][6] = type[1][7] = type[3][4] = type[3][6] =
                type[4][2] = type[4][1] = type[6][3] = type[6][4] = type[6][5] =
                    type[6][6] = type[6][7] = Grid::Type::BlockTower;
        map = new towerdefence::core::Map(
            width, height,
            [&](size_t x, size_t y) -> Grid { return Grid(type[x][y]); });
        break;
    case 3:
        type[0][0] = type[0][5] = type[0][6] = type[1][11] = type[2][0] =
            Grid::Type::BlockOut;
        type[3][11] = type[4][11] = type[6][0] = Grid::Type::BlockIn;
        type[0][4] = type[3][1] = type[6][7] = type[6][11] =
            Grid::Type::BlockTransport;
        type[0][8] = type[0][7] = type[0][9] = type[0][10] = type[0][11] =
            type[1][0] = type[1][1] = type[1][2] = type[3][0] = type[4][0] =
                type[5][0] = type[5][4] = type[5][5] = type[5][6] =
                    type[5][10] = type[5][11] = type[6][3] = type[6][4] =
                        type[6][5] = type[6][6] = Grid::Type::None;
        type[2][7] = type[1][3] = type[1][4] = type[2][8] = type[2][9] =
            type[2][10] = type[2][11] = type[4][1] = type[4][3] = type[4][4] =
                type[5][1] = type[5][3] = type[5][7] = type[5][8] = type[5][9] =
                    Grid::Type::BlockTower;
        map = new towerdefence::core::Map(
            width, height,
            [&](size_t x, size_t y) -> Grid { return Grid(type[x][y]); });
        break;
    default:
        break;
    }
}

void LevelScene::onMouseDown(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *)event;
    float x = e->getCursorX();
    float y = e->getCursorY();

    auto particle = ParticleSystemQuad::create("particles/mouse.plist");
    if (particle == nullptr) {
        problemLoading("'particles/mouse.plist'");
    } else {
        particle->setPosition(Vec2(x, y));
        this->addChild(particle, 5);
    }
    this->hideTowerInfo(x, y);
    if (this->isSelecting && this->selectedTower) {
        this->putTower(x, y);
    }
    if (!this->isSelecting && !this->isShowingTowerInfo) {
        this->showTowerInfo(x, y);
    }
}

void LevelScene::onMouseUp(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *)event;
    if (this->isSelecting && this->selectedTower &&
        e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        this->cancelSelect();
    }
}

void LevelScene::onMouseMove(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *)event;
    float x = e->getCursorX();
    float y = e->getCursorY();

    if (this->isSelecting && this->selectedTower) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float SIZE = 140;
        float typeX = origin.x + 350 + SIZE;
        float typeY = origin.y + visibleSize.height - SIZE;
        this->selectedTower->setVisible(true);
        if (x >= typeX - 0.5f * SIZE && x <= typeX + 11.5f * SIZE &&
            y >= typeY - 6.5f * SIZE && y <= typeY + 0.5f * SIZE) {
            int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
            int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
            if (this->type[indexY][indexX] == Grid::Type::BlockTower) {
                this->selectedTower->setPosition(
                    Vec2(typeX + indexX * SIZE, typeY - indexY * SIZE));
            } else {
                this->selectedTower->setPosition(Vec2(x, y));
            }
        } else {
            this->selectedTower->setPosition(Vec2(x, y));
        }
    }
}