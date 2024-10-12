#include "LevelScene.h"
#include "core/entity/entity.h"
#include "core/entity/tower/archer_base.h"
#include "core/entity/tower/archer_plus.h"
#include "core/entity/tower/highspeed_archer_plus.h"
#include "core/entity/tower/bomber_plus.h"
#include "core/entity/tower/helper_base.h"
#include "core/entity/tower/magician_base.h"
#include "core/entity/tower/core_magician_plus.h"
#include "core/entity/tower/special_magician_plus.h"
#include "core/entity/tower/diffusive_magician_plus.h"
#include "core/entity/tower/decelerate_magician_plus.h"
#include "core/entity/tower/weaken_magician_plus.h"
#include "core/entity/tower/aggressive_magician_plus.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using towerdefence::core::Tower;
using towerdefence::core::ArcherBase;
using towerdefence::core::ArcherPlus;
using towerdefence::core::HighspeedArcherPlus;
using towerdefence::core::BomberPlus;
using towerdefence::core::MagicianBase;
using towerdefence::core::CoreMagicianPlus;
using towerdefence::core::DiffusiveMagicianPlus;
using towerdefence::core::SpecialMagicianPlus;
using towerdefence::core::HelperBase;
using towerdefence::core::DecelerateMagicianPlus;
using towerdefence::core::WeakenMagicianPlus;
using towerdefence::core::AggressiveMagicianPlus;
using towerdefence::core::TowerType;

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in Level1Scene.cpp\n");
}

Sprite* LevelScene::getTower(Id id) {
    for (auto &pair : this->towers) {
        if (pair.first == id) {
            return pair.second;
        }
    }
}

Sprite* LevelScene::getEnemy(Id id) {
    for (auto &pair : this->enemies) {
        if (pair.first == id) {
            return pair.second;
        }
    }
}

void LevelScene::updateSelectorEnabled() {
    if (this->map->cost_ < this->archerBaseCost) {
        this->archerBaseSelector->setEnabled(false);
    } else {
        this->archerBaseSelector->setEnabled(true);
    }
    if (this->map->cost_ < this->magicianBaseCost) {
        this->magicianBaseSelector->setEnabled(false);
    } else {
        this->magicianBaseSelector->setEnabled(true);
    }
    if (this->map->cost_ < this->helperBaseCost) {
        this->helperBaseSelector->setEnabled(false);
    } else {
        this->helperBaseSelector->setEnabled(true);
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
                newTowerSprite->setPosition(Vec2(typeX + indexX * SIZE, typeY - indexY * SIZE));
                this->addChild(newTowerSprite, 3);
                this->towers.push_back(std::make_pair(
                    id.value(),
                    newTowerSprite)); // todo: handle spawning failure
                this->moneyLabel->setString(std::to_string(this->map->cost_));
                this->updateSelectorEnabled();
            }
            scheduleOnce([this](float dt) {
                this->cancelSelect();
            }, 0.1f, "cancelSelect");
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
            Id towerId = this->map->get_ref(indexY, indexX).grid.tower.value()->id;
            this->selectedTowerId = towerId;
            Sprite *towerSprite = this->getTower(towerId);
            if (towerSprite) {
                float towerX = towerSprite->getPositionX();
                float towerY = towerSprite->getPositionY();
                
                this->towerInfoButton->setPosition(Vec2(towerX, towerY));
                this->towerInfoButton->setScale(0.1f);
                this->towerInfoButton->setVisible(true);
                
                this->deleteButton->setPosition(Vec2(towerX, towerY));
                this->deleteButton->setScale(0.1f);
                this->deleteButton->setVisible(true);
                
                this->upgradeButton->setPosition(Vec2(towerX, towerY));
                this->upgradeButton->setScale(0.1f);
                this->upgradeButton->setVisible(true);
                
                TowerType towerType = this->map->get_ref(indexY, indexX).grid.tower.value()->status().tower_type_;
                std::string skillIconPath;
                switch (towerType) {
                    case TowerType::ArcherBase:
                        skillIconPath = "images/towers/skill_icon/archer_base";
                        if (this->map->cost_ < this->archerCost &&
                            this->map->cost_ < this->highspeedArcherCost &&
                            this->map->cost_ < this->bomberCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::MagicianBase:
                        skillIconPath = "images/towers/skill_icon/magician_base";
                        if (this->map->cost_ < this->coreMagicianCost &&
                            this->map->cost_ < this->diffusiveMagicianCost &&
                            this->map->cost_ < this->specialMagicianCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::HelperBase:
                        skillIconPath = "images/towers/skill_icon/helper_base";
                        if (this->map->cost_ < this->decelerateMagicianCost &&
                            this->map->cost_ < this->weakenMagicianCost &&
                            this->map->cost_ < this->aggressiveMagicianCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::Archer:
                        skillIconPath = "images/towers/skill_icon/archer";
                        if (this->map->cost_ < this->archerProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::ArcherPlus:
                        skillIconPath = "images/towers/skill_icon/archer_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::Bomber:
                        skillIconPath = "images/towers/skill_icon/bomber";
                        if (this->map->cost_ < this->bomberProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::BomberPlus:
                        skillIconPath = "images/towers/skill_icon/bomber_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::CoreMagician:
                        skillIconPath = "images/towers/skill_icon/core_magician";
                        if (this->map->cost_ < this->coreMagicianProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::CoreMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/core_magician_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::DecelerateMagician:
                        skillIconPath = "images/towers/skill_icon/decelerate_magician";
                        if (this->map->cost_ < this->decelerateMagicianProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::DecelerateMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/decelerate_magician_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::DiffusiveMagician:
                        skillIconPath = "images/towers/skill_icon/diffusive_magician";
                        if (this->map->cost_ < this->diffusiveMagicianProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::DiffusiveMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/diffusive_magician_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::HighspeedArcher:
                        skillIconPath = "images/towers/skill_icon/highspeed_archer";
                        if (this->map->cost_ < this->highspeedArcherProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::HighspeedArcherPlus:
                        skillIconPath = "images/towers/skill_icon/highspeed_archer_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::SpecialMagician:
                        skillIconPath = "images/towers/skill_icon/special_magician";
                        if (this->map->cost_ < this->specialMagicianProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::SpecialMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/special_magician_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::WeakenMagician:
                        skillIconPath = "images/towers/skill_icon/weaken_magician";
                        if (this->map->cost_ < this->weakenMagicianProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::WeakenMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/weaken_magician_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    case TowerType::AggressiveMagician:
                        skillIconPath = "images/towers/skill_icon/aggressive_magician";
                        if (this->map->cost_ < this->aggressiveMagicianProCost) {
                            this->upgradeButton->setEnabled(false);
                        } else {
                            this->upgradeButton->setEnabled(true);
                        }
                        break;
                    case TowerType::AggressiveMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/aggressive_magician_pro";
                        this->upgradeButton->setEnabled(false);
                        break;
                    default:
                        break;
                }
                this->skillButton->setScale(1.0f);
                this->skillButton->loadTextures(
                        skillIconPath + ".png",
                        skillIconPath + ".png",
                        skillIconPath + "_inactive.png"
                );
                this->skillButton->setPosition(Vec2(towerX, towerY));
                this->skillButton->setScale(0.1f);
                this->skillButton->setVisible(true);
                
                auto scale = ScaleTo::create(0.2f, 1.0f);
                auto move1 = MoveBy::create(0.2f, Vec2(50, 90));
                auto move2 = MoveBy::create(0.2f, Vec2(-50, 90));
                auto move3 = MoveBy::create(0.2f, Vec2(50, -90));
                auto move4 = MoveBy::create(0.2f, Vec2(-50, -90));

                auto towerInfoSpawn = Spawn::create(scale, move3, nullptr);
                auto deleteSpawn = Spawn::create(scale->clone(), move2, nullptr);
                auto upgradeSpawn = Spawn::create(scale->clone(), move4, nullptr);
                auto skillSpawn = Spawn::create(scale->clone(), move1, nullptr);
                
                this->towerInfoButton->runAction(towerInfoSpawn);
                this->deleteButton->runAction(deleteSpawn);
                this->upgradeButton->runAction(upgradeSpawn);
                this->skillButton->runAction(skillSpawn);
                this->isShowingTowerInfo = true;
                scheduleOnce([this](float dt) {
                    this->isShowingTowerInfo = false;
                }, 0.2f, "preventHighSpeed");
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
    
    auto scale = ScaleTo::create(0.2f, 0.1f);
    auto move1 = MoveBy::create(0.2f, Vec2(-50, -90));
    auto move2 = MoveBy::create(0.2f, Vec2(50, -90));
    auto move3 = MoveBy::create(0.2f, Vec2(-50, 90));
    auto move4 = MoveBy::create(0.2f, Vec2(50, 90));
    
    auto towerInfoSpawn = Spawn::create(scale, move3, nullptr);
    auto deleteSpawn = Spawn::create(scale->clone(), move2, nullptr);
    auto upgradeSpawn = Spawn::create(scale->clone(), move4, nullptr);
    auto skillSpawn = Spawn::create(scale->clone(), move1, nullptr);
    
    this->towerInfoButton->runAction(towerInfoSpawn);
    this->deleteButton->runAction(deleteSpawn);
    this->upgradeButton->runAction(upgradeSpawn);
    this->skillButton->runAction(skillSpawn);
    this->isShowingTowerInfo = true;
    scheduleOnce([this](float dt) {
        this->towerInfoButton->setVisible(false);
        this->deleteButton->setVisible(false);
        this->upgradeButton->setVisible(false);
        this->skillButton->setVisible(false);
        this->isShowingTowerInfo = false;
    }, 0.2f, "hideTowerInfo");
}

void LevelScene::showUpgradeMenu() {
    this->upgradeBackground1->setScale(0.1f);
    this->upgradeBackground1->setVisible(true);
    this->upgradeBackground2->setScale(0.1f);
    this->upgradeBackground2->setVisible(true);
    this->upgradeBackground3->setScale(0.1f);
    this->upgradeBackground3->setVisible(true);
    this->upgradeTower1->setScale(0.1f);
    this->upgradeTower1->setVisible(true);
    this->upgradeTower2->setScale(0.1f);
    this->upgradeTower2->setVisible(true);
    this->upgradeTower3->setScale(0.1f);
    this->upgradeTower3->setVisible(true);
    this->upgradeMenu->setScale(0.1f);
    this->upgradeMenu->setVisible(true);
    
    auto scale = ScaleTo::create(0.2f, 1.0f);
    auto scaleEase = EaseBackOut::create(scale->clone());
    this->upgradeBackground1->runAction(scaleEase);
    this->upgradeBackground2->runAction(scaleEase->clone());
    this->upgradeBackground3->runAction(scaleEase->clone());
    this->upgradeTower1->runAction(scaleEase->clone());
    this->upgradeTower2->runAction(scaleEase->clone());
    this->upgradeTower3->runAction(scaleEase->clone());
    this->upgradeMenu->runAction(scaleEase->clone());
}

void LevelScene::hideUpgradeMenu() {
    auto scale = ScaleTo::create(0.2f, 0.1f);
    this->upgradeBackground1->runAction(scale);
    this->upgradeBackground2->runAction(scale->clone());
    this->upgradeBackground3->runAction(scale->clone());
    this->upgradeTower1->runAction(scale->clone());
    this->upgradeTower2->runAction(scale->clone());
    this->upgradeTower3->runAction(scale->clone());
    this->upgradeMenu->runAction(scale->clone());
    scheduleOnce([this](float dt) {
        this->upgradeBackground1->setVisible(false);
        this->upgradeBackground2->setVisible(false);
        this->upgradeBackground3->setVisible(false);
        this->upgradeTower1->setVisible(false);
        this->upgradeTower2->setVisible(false);
        this->upgradeTower3->setVisible(false);
        this->upgradeMenu->setVisible(false);
    }, 0.2f, "hideUpgradeMenu");
}

void LevelScene::deleteTower(bool isReturn) {
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
    if (isReturn) {
        this->map->withdraw_tower(this->selectedTowerId);
        this->moneyLabel->setString(std::to_string(this->map->cost_));
        this->updateSelectorEnabled();
    } else {
        this->map->remove_tower(this->selectedTowerId);
    }
}

void LevelScene::upgradeTower() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + SIZE;
    float typeY = origin.y + visibleSize.height - SIZE;
    Sprite *towerSprite = this->getTower(this->selectedTowerId);
    float x = towerSprite->getPositionX();
    float y = towerSprite->getPositionY();
    int indexX = (int)((x - typeX + 0.5f * SIZE) / SIZE);
    int indexY = (int)((typeY - y + 0.5f * SIZE) / SIZE);
    std::string path;
    towerdefence::core::TowerFactoryBase *newTower;
    switch (this->map->get_ref(indexY, indexX).grid.tower.value()->status().tower_type_) {
        case TowerType::ArcherBase:
            this->upgradeTower1->setTexture("images/towers/archer.png");
            this->upgradeTower2->setTexture("images/towers/highspeed_archer.png");
            this->upgradeTower3->setTexture("images/towers/bomber.png");
            if (this->map->cost_ < this->archerCost) {
                this->upgradeItem1->setEnabled(false);
            } else {
                this->upgradeItem1->setEnabled(true);
            }
            if (this->map->cost_ < this->highspeedArcherCost) {
                this->upgradeItem2->setEnabled(false);
            } else {
                this->upgradeItem2->setEnabled(true);
            }
            if (this->map->cost_ < this->bomberCost) {
                this->upgradeItem3->setEnabled(false);
            } else {
                this->upgradeItem3->setEnabled(true);
            }
            this->showUpgradeMenu();
            return;
        case TowerType::MagicianBase:
            this->upgradeTower1->setTexture("images/towers/core_magician.png");
            this->upgradeTower2->setTexture("images/towers/diffusive_magician.png");
            this->upgradeTower3->setTexture("images/towers/special_magician.png");
            if (this->map->cost_ < this->coreMagicianCost) {
                this->upgradeItem1->setEnabled(false);
            } else {
                this->upgradeItem1->setEnabled(true);
            }
            if (this->map->cost_ < this->diffusiveMagicianCost) {
                this->upgradeItem2->setEnabled(false);
            } else {
                this->upgradeItem2->setEnabled(true);
            }
            if (this->map->cost_ < this->specialMagicianCost) {
                this->upgradeItem3->setEnabled(false);
            } else {
                this->upgradeItem3->setEnabled(true);
            }
            this->showUpgradeMenu();
            return;
        case TowerType::HelperBase:
            this->upgradeTower1->setTexture("images/towers/decelerate_magician.png");
            this->upgradeTower2->setTexture("images/towers/weaken_magician.png");
            this->upgradeTower3->setTexture("images/towers/aggressive_magician.png");
            if (this->map->cost_ < this->decelerateMagicianCost) {
                this->upgradeItem1->setEnabled(false);
            } else {
                this->upgradeItem1->setEnabled(true);
            }
            if (this->map->cost_ < this->weakenMagicianCost) {
                this->upgradeItem2->setEnabled(false);
            } else {
                this->upgradeItem2->setEnabled(true);
            }
            if (this->map->cost_ < this->aggressiveMagicianCost) {
                this->upgradeItem3->setEnabled(false);
            } else {
                this->upgradeItem3->setEnabled(true);
            }
            this->showUpgradeMenu();
            return;
        case TowerType::Archer:
            path = "images/towers/archer_pro.png";
            newTower = new towerdefence::core::TowerFactory<ArcherPlus>();
            break;
        case TowerType::Bomber:
            path = "images/towers/bomber_pro.png";
            newTower = new towerdefence::core::TowerFactory<BomberPlus>();
            break;
        case TowerType::CoreMagician:
            path = "images/towers/core_magician_pro.png";
            newTower = new towerdefence::core::TowerFactory<CoreMagicianPlus>();
            break;
        case TowerType::DecelerateMagician:
            path = "images/towers/decelerate_magician_pro.png";
            newTower = new towerdefence::core::TowerFactory<DecelerateMagicianPlus>();
            break;
        case TowerType::DiffusiveMagician:
            path = "images/towers/diffusive_magician_pro.png";
            newTower = new towerdefence::core::TowerFactory<DiffusiveMagicianPlus>();
            break;
        case TowerType::HighspeedArcher:
            path = "images/towers/highspeed_archer_pro.png";
            newTower = new towerdefence::core::TowerFactory<HighspeedArcherPlus>();
            break;
        case TowerType::SpecialMagician:
            path = "images/towers/special_magician_pro.png";
            newTower = new towerdefence::core::TowerFactory<SpecialMagicianPlus>();
            break;
        case TowerType::WeakenMagician:
            path = "images/towers/weaken_magician_pro.png";
            newTower = new towerdefence::core::TowerFactory<WeakenMagicianPlus>();
            break;
        case TowerType::AggressiveMagician:
            path = "images/towers/aggressive_magician_pro.png";
            newTower = new towerdefence::core::TowerFactory<AggressiveMagicianPlus>();
            break;
        default:
            return;
    }
    this->deleteTower(false);
    auto id = this->map->spawn_tower_at(indexY, indexX, *newTower);
    Sprite *newTowerSprite = Sprite::create(path);
    newTowerSprite->setPosition(Vec2(x, y));
    this->addChild(newTowerSprite, 3);
    this->selectedTowerId = id.value();
    this->towers.push_back(std::make_pair(id.value(), newTowerSprite));
    this->moneyLabel->setString(std::to_string(this->map->cost_));
    this->updateSelectorEnabled();
}

void LevelScene::showTowerInfo() {}

void LevelScene::executeSkill() {}

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
    EventMouse const *e = (EventMouse *)event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    
    auto particle = ParticleSystemQuad::create("particles/mouse.plist");
    if (particle == nullptr) {
        problemLoading("'particles/mouse.plist'");
    } else {
        particle->setPosition(Vec2(x, y));
        this->addChild(particle, 10);
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
    EventMouse const *e = (EventMouse*)event;
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
                this->selectedTower->setPosition(Vec2(typeX + indexX * SIZE, typeY - indexY * SIZE));
            } else {
                this->selectedTower->setPosition(Vec2(x, y));
            }
        } else {
            this->selectedTower->setPosition(Vec2(x, y));
        }
    }
}