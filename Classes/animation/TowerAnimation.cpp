//
// Created by Ignorant on 2024/10/12.
//

#include "TowerAnimation.h"

Bullet::Bullet(LevelScene *levelScene, towerdefence::core::Tower *tower, towerdefence::core::Enemy *enemy) :
levelScene(levelScene), tower(tower), enemy(enemy) {
    switch (this->tower->status().tower_type_) {
        case TowerType::ArcherBase:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/arrow_basic.png");
            break;
        case TowerType::Archer:
        case TowerType::ArcherPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/archer_arrow.png");
            break;
        case TowerType::HighspeedArcher:
        case TowerType::HighspeedArcherPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/highspeed_arrow.png");
            break;
        case TowerType::MagicianBase:
        case TowerType::DecelerateMagician:
        case TowerType::DecelerateMagicianPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/blueBullet/blueBullet00.png");
            break;
        case TowerType::CoreMagician:
        case TowerType::CoreMagicianPlus:
        case TowerType::WeakenMagician:
        case TowerType::WeakenMagicianPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/purpleBullet/purpleBullet00.png");
            break;
        case TowerType::DiffusiveMagician:
        case TowerType::DiffusiveMagicianPlus:
        case TowerType::HelperBase:
            this->bullet = cocos2d::Sprite::create("images/bullet/greenBullet/greenBullet00.png");
            break;
        case TowerType::SpecialMagician:
        case TowerType::SpecialMagicianPlus:
        case TowerType::AggressiveMagician:
        case TowerType::AggressiveMagicianPlus:
            this->bullet = cocos2d::Sprite::create("images/bullet/orangeBullet/orangeBullet00.png");
            break;
        default:
            this->bullet = cocos2d::Sprite::create("images/bullet/arrows/arrow_basic.png");
            break;
    }
    Id towerId = this->tower->id;
    Id enemyId = this->enemy->id;
    auto *towerSprite = this->levelScene->getTower(towerId);
    float towerX = towerSprite->getPositionX();
    float towerY = towerSprite->getPositionY();
    auto *enemySprite = this->levelScene->getEnemy(enemyId);
    float enemyX = enemySprite->getPositionX();
    float enemyY = enemySprite->getPositionY();
    this->bullet->setPosition(towerX, towerY);
    this->dx = (enemyX - towerX) / this->speed;
    this->dy = (enemyY - towerY) / this->speed;
    if (enemyX == towerX) {
        if (enemyY < towerY) {
            this->angle = 90.0f;
        } else {
            this->angle = -90.0f;
        }
    } else {
        this->angle = (float) (atan((towerY - enemyY) / (enemyX - towerX)) * 180.0f / M_PI);
    }
    this->bullet->setRotation(this->angle);
    this->levelScene->addBullet(this);
}

void Bullet::move() {
    this->updateAngle();
    this->bullet->setRotation(this->angle);
    this->bullet->setPositionX(this->bullet->getPositionX() + this->dx);
    this->bullet->setPositionX(this->bullet->getPositionX() + this->dx);
}

void Bullet::updateAngle() {
    Id enemyId = this->enemy->id;
    float x = this->bullet->getPositionX();
    float y = this->bullet->getPositionY();
    auto *enemySprite = this->levelScene->getEnemy(enemyId);
    float enemyX = enemySprite->getPositionX();
    float enemyY = enemySprite->getPositionY();
    this->bullet->setPosition(x, y);
    this->dx = (enemyX - x) / this->speed;
    this->dy = (enemyY - y) / this->speed;
    if (enemyX == x) {
        if (enemyY < y) {
            this->angle = 90.0f;
        } else {
            this->angle = -90.0f;
        }
    } else {
        this->angle = (float) (atan((y - enemyY) / (enemyX - x)) * 180.0f / M_PI);
    }
}


MagicBullet::MagicBullet(LevelScene *levelScene, towerdefence::core::Tower *tower, towerdefence::core::Enemy *enemy,
                         std::string color) : Bullet(levelScene, tower, enemy), color(color) {}

void MagicBullet::move() {
    Bullet::move();
    std::string prefix = "images/bullet/" + this->color + "Bullet/" + this->color;
    std::string bulletName = std::format("Bullet{:02d}.png", this->currentFrame);
    this->bullet->setTexture(prefix + bulletName);
    this->currentFrame = (this->currentFrame + 1) % this->totalFrames;
}

void MagicBullet::explosion() {
    this->bullet->setRotation(0);
    std::string prefix = "images/bullet/" + this->color + "Bomb/" + this->color;
    this->bullet->setTexture(prefix + "Bomb00.png");
    auto *enemySprite = this->levelScene->getEnemy(this->enemy->id);
    this->bullet->setPosition(enemySprite->getPositionX(), enemySprite->getPositionY());
    cocos2d::Vector<cocos2d::SpriteFrame*> frames;
    frames.reserve(this->totalFrames);
    for (int i = 0; i < this->totalFrames; i++) {
        std::string frameName = std::format("Bomb{:02d}.png", i);
        auto *frame = cocos2d::SpriteFrame::create(prefix + frameName, cocos2d::Rect(0, 0, 140, 140));
        frames.pushBack(frame);
    }
    auto *animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto *animate = cocos2d::Animate::create(animation);
    this->bullet->runAction(animate);
}
