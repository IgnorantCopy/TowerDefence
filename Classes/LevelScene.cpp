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
#include "core/entity/enemy/dog.h"
#include "core/entity/enemy/worm.h"
#include "core/entity/enemy/Tank.h"
#include "core/entity/enemy/Warlock.h"
#include "core/entity/enemy/Speed-up.h"
#include "core/entity/enemy/Soldier.h"
#include "core/entity/enemy/Attack-down.h"
#include "core/entity/enemy/Boss-1.h"
#include "core/entity/enemy/Boss-2.h"
#include "core/entity/enemy/Crab.h"
#include "core/entity/enemy/Destroyer.h"
#include "core/entity/enemy/Life-up.h"
#include "core/entity/enemy/Not-attacked.h"
#include "ui/CocosGUI.h"
#include "animation/EnemyAnimation.h"
#include "animation/TowerAnimation.h"
#include <any>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>


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
using towerdefence::core::TowerFactory;
using towerdefence::core::TowerFactoryBase;
using towerdefence::core::Dog;
using towerdefence::core::Worm;
using towerdefence::core::Tank;
using towerdefence::core::Warlock;
using towerdefence::core::SpeedUp;
using towerdefence::core::Soldier;
using towerdefence::core::AttackDown;
using towerdefence::core::Boss1;
using towerdefence::core::Boss2;
using towerdefence::core::Crab;
using towerdefence::core::Destroyer;
using towerdefence::core::LifeUp;
using towerdefence::core::NotAttacked;

static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
           "in front of filenames in Level1Scene.cpp\n");
}

Sprite *LevelScene::getTower(Id id) {
    for (auto &pair: this->towers) {
        if (pair.first == id) {
            return pair.second;
        }
    }
    return nullptr;
}

Sprite *LevelScene::getEnemy(Id id) {
    for (auto &pair: this->enemies) {
        if (pair.first == id) {
            return pair.second;
        }
    }
    return nullptr;
}

void LevelScene::addBullet(Bullet *bullet) {
    this->bullets.push_back(bullet);
    this->addChild(bullet->getBullet(), 4);
}

void LevelScene::updateBullets() {
    for (auto it = this->bullets.begin(); it != this->bullets.end(); it++) {
        if ((*it)->isTouch()) {
            this->bullets.erase(it);
        } else {
            (*it)->move();
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

void LevelScene::updateMoneyLabel() {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    if (this->moneyLabel != nullptr) {
        this->moneyLabel->setString(std::to_string(this->map->cost_));
        this->moneyLabel->setPosition(
                cocos2d::Vec2(origin.x + 150 + 15 * log10(map->getcost_()), origin.y + visibleSize.height - 70));
    }
}

void LevelScene::decreaseLife() {
    this->map->health_--;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    if (this->map->health_ <= 0) {
    
    } else {
        this->lifeLabel->setString(std::to_string(this->map->health_));
        this->lifeLabel->setPosition(
                cocos2d::Vec2(origin.x + 150 + 15 * log10(this->map->health_), origin.y + visibleSize.height - 180));
    }
}

void LevelScene::updateUpgradeItemEnabled() {
    if (upgradeItem1 != nullptr && upgradeItem2 != nullptr && upgradeItem3 != nullptr) {
        if (this->map->cost_ < this->upgradeTower1Cost) {
            this->upgradeItem1->setEnabled(false);
        } else {
            this->upgradeItem1->setEnabled(true);
        }
        if (this->map->cost_ < this->upgradeTower2Cost) {
            this->upgradeItem2->setEnabled(false);
        } else {
            this->upgradeItem2->setEnabled(true);
        }
        if (this->map->cost_ < this->upgradeTower3Cost) {
            this->upgradeItem3->setEnabled(false);
        } else {
            this->upgradeItem3->setEnabled(true);
        }
    }
}

void LevelScene::updateUpgradeButtonEnabled() {
    if (this->upgradeButton != nullptr) {
        if (this->map->cost_ < this->upgradeTowerCost) {
            this->upgradeButton->setEnabled(false);
        } else {
            this->upgradeButton->setEnabled(true);
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
        int indexX = (int) ((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int) ((typeY - y + 0.5f * SIZE) / SIZE);
        if (this->type[indexY][indexX] == Grid::Type::BlockTower) {
            if (!this->map->get_ref(indexY, indexX).grid.tower.has_value()) {
                std::string path = "images/towers/";
                std::unique_ptr<TowerFactoryBase> newTower;
                switch (this->isSelecting) {
                    case 1:
                        path += "archer_base_onblock.png";
                        newTower = std::make_unique<TowerFactory<ArcherBase>>();
                        break;
                    case 2:
                        path += "magician_base_onblock.png";
                        newTower = std::make_unique<TowerFactory<MagicianBase>>();
                        break;
                    case 3:
                        path += "helper_base_onblock.png";
                        newTower = std::make_unique<TowerFactory<HelperBase>>();
                        break;
                    default:
                        break;
                }
                auto id = this->map->spawn_tower_at(indexY, indexX, *newTower);
                auto newTowerSprite = Sprite::create(path);
                newTowerSprite->setPosition(Vec2(typeX + indexX * SIZE, typeY - indexY * SIZE));
                this->addChild(newTowerSprite, 3);
                this->towers.emplace_back(
                        id.value(),
                        newTowerSprite); // todo: handle spawning failure
                this->updateMoneyLabel();
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
        int indexX = (int) ((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int) ((typeY - y + 0.5f * SIZE) / SIZE);
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
                        this->upgradeTower1Cost = this->archerCost;
                        this->upgradeTower2Cost = this->highspeedArcherCost;
                        this->upgradeTower3Cost = this->bomberCost;
                        this->upgradeTowerCost = std::min(std::min(this->upgradeTower1Cost, this->upgradeTower2Cost),
                                                          this->upgradeTower3Cost);
                        break;
                    case TowerType::MagicianBase:
                        skillIconPath = "images/towers/skill_icon/magician_base";
                        this->upgradeTower1Cost = this->coreMagicianCost;
                        this->upgradeTower2Cost = this->diffusiveMagicianCost;
                        this->upgradeTower3Cost = this->specialMagicianCost;
                        this->upgradeTowerCost = std::min(std::min(this->upgradeTower1Cost, this->upgradeTower2Cost),
                                                          this->upgradeTower3Cost);
                        break;
                    case TowerType::HelperBase:
                        skillIconPath = "images/towers/skill_icon/helper_base";
                        this->upgradeTower1Cost = this->decelerateMagicianCost;
                        this->upgradeTower2Cost = this->weakenMagicianCost;
                        this->upgradeTower3Cost = this->aggressiveMagicianCost;
                        this->upgradeTowerCost = std::min(std::min(this->upgradeTower1Cost, this->upgradeTower2Cost),
                                                          this->upgradeTower3Cost);
                        break;
                    case TowerType::Archer:
                        skillIconPath = "images/towers/skill_icon/archer";
                        this->upgradeTowerCost = this->archerProCost;
                        break;
                    case TowerType::ArcherPlus:
                        skillIconPath = "images/towers/skill_icon/archer_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::Bomber:
                        skillIconPath = "images/towers/skill_icon/bomber";
                        this->upgradeTowerCost = this->bomberProCost;
                        break;
                    case TowerType::BomberPlus:
                        skillIconPath = "images/towers/skill_icon/bomber_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::CoreMagician:
                        skillIconPath = "images/towers/skill_icon/core_magician";
                        this->upgradeTowerCost = this->coreMagicianProCost;
                        break;
                    case TowerType::CoreMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/core_magician_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::DecelerateMagician:
                        skillIconPath = "images/towers/skill_icon/decelerate_magician";
                        this->upgradeTowerCost = this->decelerateMagicianProCost;
                        break;
                    case TowerType::DecelerateMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/decelerate_magician_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::DiffusiveMagician:
                        skillIconPath = "images/towers/skill_icon/diffusive_magician";
                        this->upgradeTowerCost = this->diffusiveMagicianProCost;
                        break;
                    case TowerType::DiffusiveMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/diffusive_magician_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::HighspeedArcher:
                        skillIconPath = "images/towers/skill_icon/highspeed_archer";
                        this->upgradeTowerCost = this->highspeedArcherProCost;
                        break;
                    case TowerType::HighspeedArcherPlus:
                        skillIconPath = "images/towers/skill_icon/highspeed_archer_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::SpecialMagician:
                        skillIconPath = "images/towers/skill_icon/special_magician";
                        this->upgradeTowerCost = this->specialMagicianProCost;
                        break;
                    case TowerType::SpecialMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/special_magician_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::WeakenMagician:
                        skillIconPath = "images/towers/skill_icon/weaken_magician";
                        this->upgradeTowerCost = this->weakenMagicianProCost;
                        break;
                    case TowerType::WeakenMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/weaken_magician_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    case TowerType::AggressiveMagician:
                        skillIconPath = "images/towers/skill_icon/aggressive_magician";
                        this->upgradeTowerCost = this->aggressiveMagicianProCost;
                        break;
                    case TowerType::AggressiveMagicianPlus:
                        skillIconPath = "images/towers/skill_icon/aggressive_magician_pro";
                        this->upgradeTowerCost = 1000000;
                        break;
                    default:
                        break;
                }
                updateUpgradeButtonEnabled();
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
                    scheduleOnce([this](float dt) {
                        this->isShowingTowerInfo = true;
                    }, 0.1f, "resetTowerInfo");
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
        int indexX = (int) ((x - typeX + 0.5f * SIZE) / SIZE);
        int indexY = (int) ((typeY - y + 0.5f * SIZE) / SIZE);
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
    this->isUpgrade = true;
    
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
    
    auto scale = ScaleTo::create(0.5f, 1.0f);
    auto scaleLarge = ScaleTo::create(0.5f, 2.0f);
    auto scaleEase = EaseBackOut::create(scale->clone());
    auto scaleEaseLarge = EaseBackOut::create(scaleLarge->clone());
    this->upgradeBackground1->runAction(scaleEase);
    this->upgradeBackground2->runAction(scaleEase->clone());
    this->upgradeBackground3->runAction(scaleEase->clone());
    this->upgradeTower1->runAction(scaleEaseLarge);
    this->upgradeTower2->runAction(scaleEaseLarge->clone());
    this->upgradeTower3->runAction(scaleEaseLarge->clone());
    this->upgradeMenu->runAction(scaleEase->clone());
}

void LevelScene::hideUpgradeMenu() {
    auto scale = ScaleTo::create(0.3f, 0.1f);
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
        this->isUpgrade = false;
    }, 0.3f, "hideUpgradeMenu");
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
        if (isReturn) {
            this->map->withdraw_tower(this->selectedTowerId);
            this->updateMoneyLabel();
            this->updateSelectorEnabled();
        } else {
            this->map->remove_tower(this->selectedTowerId);
        }
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
    int indexX = (int) ((x - typeX + 0.5f * SIZE) / SIZE);
    int indexY = (int) ((typeY - y + 0.5f * SIZE) / SIZE);
    std::string path;
    std::unique_ptr<TowerFactoryBase> newTower;
    switch (this->map->get_ref(indexY, indexX).grid.tower.value()->status().tower_type_) {
        case TowerType::ArcherBase:
            this->upgradeTower1->setTexture("images/towers/archer.png");
            this->upgradeTower2->setTexture("images/towers/highspeed_archer.png");
            this->upgradeTower3->setTexture("images/towers/bomber.png");
            this->upgradeTower1->setScale(2.0f);
            this->upgradeTower2->setScale(2.0f);
            this->upgradeTower3->setScale(2.0f);
            updateUpgradeItemEnabled();
            this->showUpgradeMenu();
            return;
        case TowerType::MagicianBase:
            this->upgradeTower1->setTexture("images/towers/core_magician.png");
            this->upgradeTower2->setTexture("images/towers/diffusive_magician.png");
            this->upgradeTower3->setTexture("images/towers/special_magician.png");
            this->upgradeTower1->setScale(2.0f);
            this->upgradeTower2->setScale(2.0f);
            this->upgradeTower3->setScale(2.0f);
            updateUpgradeItemEnabled();
            this->showUpgradeMenu();
            return;
        case TowerType::HelperBase:
            this->upgradeTower1->setTexture("images/towers/decelerate_magician.png");
            this->upgradeTower2->setTexture("images/towers/weaken_magician.png");
            this->upgradeTower3->setTexture("images/towers/aggressive_magician.png");
            this->upgradeTower1->setScale(2.0f);
            this->upgradeTower2->setScale(2.0f);
            this->upgradeTower3->setScale(2.0f);
            updateUpgradeItemEnabled();
            this->showUpgradeMenu();
            return;
        case TowerType::Archer:
            path = "images/towers/archer_pro.png";
            newTower = std::make_unique<TowerFactory<ArcherPlus>>();
            break;
        case TowerType::Bomber:
            path = "images/towers/bomber_pro.png";
            newTower = std::make_unique<TowerFactory<BomberPlus>>();
            break;
        case TowerType::CoreMagician:
            path = "images/towers/core_magician_pro.png";
            newTower = std::make_unique<TowerFactory<CoreMagicianPlus>>();
            break;
        case TowerType::DecelerateMagician:
            path = "images/towers/decelerate_magician_pro.png";
            newTower = std::make_unique<TowerFactory<DecelerateMagicianPlus>>();
            break;
        case TowerType::DiffusiveMagician:
            path = "images/towers/diffusive_magician_pro.png";
            newTower = std::make_unique<TowerFactory<DiffusiveMagicianPlus>>();
            break;
        case TowerType::HighspeedArcher:
            path = "images/towers/highspeed_archer_pro.png";
            newTower = std::make_unique<TowerFactory<HighspeedArcherPlus>>();
            break;
        case TowerType::SpecialMagician:
            path = "images/towers/special_magician_pro.png";
            newTower = std::make_unique<TowerFactory<SpecialMagicianPlus>>();
            break;
        case TowerType::WeakenMagician:
            path = "images/towers/weaken_magician_pro.png";
            newTower = std::make_unique<TowerFactory<WeakenMagicianPlus>>();
            break;
        case TowerType::AggressiveMagician:
            path = "images/towers/aggressive_magician_pro.png";
            newTower = std::make_unique<TowerFactory<AggressiveMagicianPlus>>();
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
    this->towers.emplace_back(id.value(), newTowerSprite);
    this->updateMoneyLabel();
    this->updateSelectorEnabled();
}

void LevelScene::showTowerInfo() {}

void LevelScene::executeSkill() {}

void LevelScene::menuCloseCallback(cocos2d::Ref *pSender) {
    //Close the cocos2d-x game scene and quit the application
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
            gridType = {
                    {2, 0, 0, 0, 0, 5, 5, 3, 0, 0, 0, 2},
                    {5, 4, 4, 4, 0, 4, 4, 4, 5, 5, 5, 5},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                    {1, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 2},
                    {5, 4, 4, 4, 4, 0, 4, 5, 4, 4, 4, 5},
                    {2, 0, 0, 0, 0, 0, 5, 3, 0, 0, 0, 1}
            };
            for (size_t i = 0; i < height; i++) {
                for (size_t j = 0; j < width; j++) {
                    type[i][j] = gridTypes[gridType[i][j]];
                }
            }
            map = new towerdefence::core::Map(width, height,
                                              [&](size_t x, size_t y) -> Grid { return Grid(type[x][y]); });
            routes = {
                    Route({Dir[R], Dir[R], Dir[R], Dir[R], Dir[D], Dir[D], Dir[D], Dir[D], Dir[L], Dir[L],
                           Dir[L], Dir[U], Dir[U], Dir[R], Dir[R], Dir[D], Dir[D], Dir[L], Dir[L], Dir[L]}),
                    Route({Dir[L], Dir[L], Dir[L], Dir[L], {6, 0}, Dir[R], Dir[R], Dir[R], Dir[R]}),
                    Route({Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L]}),
                    Route({Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[L], Dir[D], Dir[L], Dir[L],
                           Dir[U], Dir[L]}),
                    Route({Dir[L], Dir[L], Dir[L], Dir[U], Dir[L], Dir[L], Dir[L], Dir[D], Dir[L], Dir[L], Dir[L],
                           Dir[L], Dir[L]}),
                    Route({Dir[R], Dir[R], Dir[R], Dir[R], Dir[R], Dir[U], Dir[U], Dir[U], Dir[U], Dir[L],
                           Dir[L], Dir[D], Dir[D], Dir[R], Dir[R], Dir[U], Dir[U], Dir[L], Dir[L], Dir[L], Dir[L],
                           Dir[L]})
            };
            enemyCreateTime = {10.0, 11.0, 12.0, 15.0, 16.0, 17.0, 20.0, 22.0, 24.0, 30.0, 33.0, 36.0, 39.0, 45.0, 46.0,
                               47.0, 55.0, 57.0,
                               59.0, 65.0, 68.0, 71.0, 72.0, 77.0, 80.0, 85.0, 95.0, 101.0, 110.0, 120.0, 130.0, 145.0,
                               160.0, 180.0, 200.0};
            enemyNumber = 167;
            enemyStartPos = {{0, 0},
                             {0, 0},
                             {0, 11},
                             {2, 11},
                             {3, 11},
                             {4, 11},
                             {6, 0}};
            enemyCreateType = {
                    {{3, 2}},
                    {{3, 2}},
                    {{3, 2}},
                    {{3, 2}, {4, 1}, {5, 1}},
                    {{3, 2}, {4, 1}, {5, 1}},
                    {{3, 2}, {4, 1}, {5, 1}},
                    {{3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}},
                    {{3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}},
                    {{3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}},
                    {{3, 3}, {3, 4}, {4, 3}, {4, 4}},
                    {{3, 3}, {3, 4}, {4, 3}, {4, 4}},
                    {{3, 3}, {3, 4}, {4, 3}, {4, 4}, {5, 3}, {5, 4}},
                    {{3, 3}, {3, 4}, {4, 3}, {4, 4}, {5, 3}, {5, 4}},
                    {{1, 7}, {3, 2}, {4, 1}, {5, 1}},
                    {{1, 7}, {3, 2}, {4, 1}, {5, 1}},
                    {{1, 7}, {3, 2}, {4, 1}, {5, 1}},
                    {{1, 7}, {3, 3}, {4, 3}, {5, 4}},
                    {{1, 7}, {3, 3}, {4, 3}, {5, 4}},
                    {{1, 7}, {3, 3}, {4, 3}, {5, 4}},
                    {{1, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}},
                    {{1, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}},
                    {{1, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}},
                    {{1, 5}},
                    {{6, 6}},
                    {{1, 5}, {2, 7}, {3, 2}, {4, 1}, {5, 1}, {6, 6}},
                    {{1, 5}, {2, 7}, {3, 2}, {4, 1}, {5, 1}, {6, 6}},
                    {{1, 5}, {2, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}},
                    {{2, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}, {6, 6}},
                    {{1, 5}, {2, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}, {6, 6}},
                    {{1, 5}, {2, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}, {6, 6}},
                    {{1, 5}, {2, 7}, {3, 2}, {3, 3}, {4, 1}, {4, 3}, {5, 1}, {5, 3}, {6, 6}},
                    {{2, 7}, {3, 5}, {4, 8}, {5, 6}},
                    {{2, 7}, {3, 5}, {4, 8}, {5, 6}},
                    {{1, 9}, {3, 5}, {4, 8}, {5, 6}},
                    {{1, 9}, {2, 7}, {3, 5}, {4, 8}, {5, 6}, {6, 9}}
            };
            break;
        case 2:
            type[2][11] = type[0][11] = type[3][11] = type[4][11] = type[6][11] = Grid::Type::BlockOut;
            type[2][1] = type[3][1] = type[6][1] = Grid::Type::BlockIn;
            type[0][0] = type[0][8] = type[6][8] = Grid::Type::BlockTransport;
            type[0][1] = type[0][7] = type[1][1] = type[1][8] = type[1][9] = type[1][10] =
            type[1][11] = type[5][1] = type[5][8] = type[5][11] = type[5][9] = type[5][10] = Grid::Type::None;
            type[0][2] = type[0][3] = type[0][4] = type[0][5] = type[0][6] = type[1][2] = type[1][6] = type[1][7] =
            type[3][4] = type[3][6] = type[4][2] = type[4][1] = type[6][3] = type[6][4] = type[6][5] = type[6][6] = type[6][7] = Grid::Type::BlockTower;
            map = new towerdefence::core::Map(width, height,
                                              [&](size_t x, size_t y) -> Grid { return Grid(type[x][y]); });
            break;
        case 3:
            type[0][0] = type[0][5] = type[0][6] = type[1][11] = type[2][0] = Grid::Type::BlockOut;
            type[3][11] = type[4][11] = type[6][0] = Grid::Type::BlockIn;
            type[0][4] = type[3][1] = type[6][7] = type[6][11] = Grid::Type::BlockTransport;
            type[0][8] = type[0][7] = type[0][9] = type[0][10] = type[0][11] = type[1][0] =
            type[1][1] = type[1][2] = type[3][0] = type[4][0] = type[5][0] = type[5][4] = type[5][5] =
            type[5][6] = type[5][10] = type[5][11] = type[6][3] = type[6][4] = type[6][5] = type[6][6] = Grid::Type::None;
            type[2][7] = type[1][3] = type[1][4] = type[2][8] = type[2][9] = type[2][10] = type[2][11] = type[4][1] =
            type[4][3] = type[4][4] = type[5][1] = type[5][3] = type[5][7] = type[5][8] = type[5][9] = Grid::Type::BlockTower;
            map = new towerdefence::core::Map(width, height,
                                              [&](size_t x, size_t y) -> Grid { return Grid(type[x][y]); });
            break;
        default:
            break;
    }
    
    this->map->on_enemy_move(
            [this](Enemy &enemy, std::pair<size_t, size_t> currentPos, std::pair<size_t, size_t> targetPos) {
                EnemyAnimation::move(this, &enemy, currentPos, targetPos);
            });
    this->map->on_enemy_attacked([this](Enemy &enemy, Tower &tower) {
    });
    this->map->on_enemy_death([this](Enemy &enemy) {
        EnemyAnimation::dead(this, &enemy);
    });
    this->map->on_escape([this](Id id) {
        auto enemySprite = this->getEnemy(id);
        if (enemySprite) {
            enemySprite->removeFromParent();
        }
        this->decreaseLife();
    });
    this->map->on_enemy_release_skill([this](const Enemy &enemy, towerdefence::core::Map &map, uint32_t duration) {
    
    });
}

void LevelScene::onMouseDown(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *) event;
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
    if (!this->isSelecting && !this->isShowingTowerInfo && !this->isUpgrade) {
        this->showTowerInfo(x, y);
    }
}

void LevelScene::onMouseUp(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *) event;
    if (this->isSelecting && this->selectedTower &&
        e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        this->cancelSelect();
    }
}

void LevelScene::onMouseMove(cocos2d::Event *event) {
    EventMouse const *e = (EventMouse *) event;
    float x = e->getCursorX();
    float y = e->getCursorY();
    
    if (this->isSelecting && this->selectedTower) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        float typeX = origin.x + 350 + SIZE;
        float typeY = origin.y + visibleSize.height - SIZE;
        this->selectedTower->setVisible(true);
        if (x >= typeX - 0.5f * SIZE && x <= typeX + 11.5f * SIZE &&
            y >= typeY - 6.5f * SIZE && y <= typeY + 0.5f * SIZE) {
            int indexX = (int) ((x - typeX + 0.5f * SIZE) / SIZE);
            int indexY = (int) ((typeY - y + 0.5f * SIZE) / SIZE);
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

void LevelScene::createEnemy() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float X = origin.x + 350 + SIZE;
    float Y = origin.y + visibleSize.height - SIZE;
    for (size_t i = 0; i < enemyCreateTime.size(); i++) {
        for (size_t j = 0; j < enemyCreateType[i].size(); j++) {
            std::string enemyPath = "images/enemies/";
            size_t x = enemyStartPos[enemyCreateType[i][j].first].first;
            size_t y = enemyStartPos[enemyCreateType[i][j].first].second;
            enemyPos.emplace_back(x, y);
            Route new_route = routes[enemyCreateType[i][j].first - 1];
            auto extra_storage = 
                std::unordered_map<std::string, std::any>{{"current_frame", 0}};
            std::unique_ptr<EnemyFactoryBase> newEnemy;
            
            switch (enemyType[enemyCreateType[i][j].second - 1]) {
                case EnemyType::Dog:
                    enemyPath += "dog/move/dog_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Dog>>(new_route, extra_storage);
                    break;
                case EnemyType::Soldier:
                    enemyPath += "soldier/move/soldier_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Soldier>>(new_route, extra_storage);
                    break;
                case EnemyType::Worm:
                    enemyPath += "worm/move/worm_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Worm>>(new_route, extra_storage);
                    break;
                case EnemyType::Warlock:
                    enemyPath += "warlock/move/warlock_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Warlock>>(new_route, extra_storage);
                    break;
                case EnemyType::Destroyer:
                    enemyPath += "destroyer/move/destroyer_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Destroyer>>(new_route, extra_storage);
                    break;
                case EnemyType::Tank:
                    enemyPath += "tank/move/tank_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Tank>>(new_route, extra_storage);
                    break;
                case EnemyType::Crab:
                    enemyPath += "crab/move/crab_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Crab>>(new_route, extra_storage);
                    break;
                case EnemyType::SpeedUp:
                    enemyPath += "speedUp/move/speedUp_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<SpeedUp>>(new_route, extra_storage);
                    break;
                case EnemyType::AttackDown:
                    enemyPath += "attackDown/move/attackDown_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<AttackDown>>(new_route, extra_storage);
                    break;
                case EnemyType::LifeUp:
                    enemyPath += "lifeUp/move/lifeUp_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<LifeUp>>(new_route, extra_storage);
                    break;
                case EnemyType::NotAttacked:
                    enemyPath += "notAttacked/move/notAttacked_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<NotAttacked>>(new_route, extra_storage);
                    break;
                case EnemyType::Boss1:
                    enemyPath += "boss/stage1/move/boss1_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Boss1>>(new_route, extra_storage);
                    break;
                case EnemyType::Boss2:
                    enemyPath += "boss/stage2/move/boss2_move00.png";
                    newEnemy = std::make_unique<EnemyFactory<Boss2>>(new_route, extra_storage);
                    break;
                default:
                    break;
            }
            
            enemyFactories.push_back(std::move(newEnemy));
            auto newEnemySprite = Sprite::create(enemyPath);
            newEnemySprite->setScale(0.25f);
            newEnemySprite->setPosition(Vec2(X + y * SIZE, Y - x * SIZE));
            newEnemySprite->setVisible(false);
            enemySprites.emplace_back(enemyCreateTime[i], newEnemySprite);
            this->addChild(newEnemySprite, 5);
        }
    }
    for (size_t i = 0; i < enemyNumber; i++) {
        scheduleOnce([this, i](float dt) {
            enemySprites[i].second->setVisible(true);
            enemies.emplace_back(this->map->spawn_enemy_at(enemyPos[i].first, enemyPos[i].second, *enemyFactories[i]),
                                 enemySprites[i].second);
        }, enemySprites[i].first, "createEnemy" + std::to_string(i));
    }
}

void LevelScene::update() {
    this->updateBullets();
    this->updateMoneyLabel();
    this->updateSelectorEnabled();
    this->updateUpgradeItemEnabled();
    this->updateUpgradeButtonEnabled();
}