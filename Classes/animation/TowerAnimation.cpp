//
// Created by Ignorant on 2024/10/12.
//

#include "TowerAnimation.h"

size_t TowerAnimation::removeCounter = 0;

static bool isInRange(int x1, int y1, int x2, int y2, int range) {
    return abs(x1 - x2) + abs(y1 - y2) <= range;
}

Bullet::Bullet(LevelScene *levelScene, towerdefence::core::Tower *tower,
               towerdefence::core::Enemy *enemy)
    : levelScene(levelScene), towerId(tower->id), enemyId(enemy->id),
      isScaling(tower->get_storage<bool>("isScaling")) {
    switch (
        this->levelScene->map->get_tower_by_id(towerId).status().tower_type_) {
    case TowerType::ArcherBase:
        this->bullet = cocos2d::makeRef(
            cocos2d::Sprite::create("images/bullet/arrows/arrow_basic.png"));
        this->bullet->setScale(0.05f);
        break;
    case TowerType::Archer:
    case TowerType::ArcherPlus:
        this->bullet = cocos2d::makeRef(
            cocos2d::Sprite::create("images/bullet/arrows/archer_arrow.png"));
        this->bullet->setScale(0.05f);
        break;
    case TowerType::HighspeedArcher:
    case TowerType::HighspeedArcherPlus:
        this->bullet = cocos2d::makeRef(cocos2d::Sprite::create(
            "images/bullet/arrows/highspeed_arrow.png"));
        this->bullet->setScale(0.05f);
        break;
    case TowerType::Bomber:
        this->explosionFrames.reserve(44);
        for (int i = 0; i < 44; i++) {
            std::string frameName =
                std::format("images/bullet/bomb/bomb{:02d}.png", i);
            auto *frame = cocos2d::SpriteFrame::create(
                frameName, cocos2d::Rect(0, 0, 1080, 1080));
            this->explosionFrames.pushBack(frame);
        }
    case TowerType::BomberPlus:
        this->bullet = cocos2d::makeRef(
            cocos2d::Sprite::create("images/bullet/arrows/bomber_arrow.png"));
        this->bullet->setScale(0.05f);
        break;
    case TowerType::MagicianBase:
    case TowerType::DecelerateMagician:
    case TowerType::DecelerateMagicianPlus:
        this->bullet = cocos2d::makeRef(cocos2d::Sprite::create(
            "images/bullet/blueBullet/blueBullet00.png"));
        this->bullet->setScale(0.2f);
        break;
    case TowerType::CoreMagician:
    case TowerType::CoreMagicianPlus:
    case TowerType::WeakenMagician:
    case TowerType::WeakenMagicianPlus:
        this->bullet = cocos2d::makeRef(cocos2d::Sprite::create(
            "images/bullet/purpleBullet/purpleBullet00.png"));
        this->bullet->setScale(0.2f);
        break;
    case TowerType::DiffusiveMagician:
    case TowerType::DiffusiveMagicianPlus:
    case TowerType::HelperBase:
        this->bullet = cocos2d::makeRef(cocos2d::Sprite::create(
            "images/bullet/greenBullet/greenBullet00.png"));
        this->bullet->setScale(0.2f);
        break;
    case TowerType::SpecialMagician:
    case TowerType::SpecialMagicianPlus:
    case TowerType::AggressiveMagician:
    case TowerType::AggressiveMagicianPlus:
        this->bullet = cocos2d::makeRef(cocos2d::Sprite::create(
            "images/bullet/orangeBullet/orangeBullet00.png"));
        this->bullet->setScale(0.2f);
        break;
    default:
        this->bullet = cocos2d::makeRef(
            cocos2d::Sprite::create("images/bullet/arrows/arrow_basic.png"));
        this->bullet->setScale(0.05f);
        break;
    }
    auto *towerSprite = this->levelScene->getTower(this->towerId);
    float towerX = towerSprite->getPositionX();
    float towerY = towerSprite->getPositionY();
    this->enemySprite =
        cocos2d::makeRef(this->levelScene->getEnemy(this->enemyId));
    if (this->enemySprite == nullptr) {
        this->bullet->removeFromParent();
        this->bullet = nullptr;
        return;
    }
    float enemyX = this->enemySprite->getPositionX();
    float enemyY = this->enemySprite->getPositionY();
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
        this->angle = (float)(atan((towerY - enemyY) / (enemyX - towerX)) *
                              180.0f / M_PI);
    }
    if (enemyX < towerX) {
        this->angle += 180.0f;
    }
    this->bullet->setRotation(this->angle);
    tower->set_storage<bool>("isScaling", false);
}

void Bullet::explosion() {
    this->isExploding = true;
    if (this->enemySprite == nullptr) {
        this->bullet->removeFromParent();
        this->bullet = nullptr;
        return;
    }
    if (this->isScaling && !this->explosionFrames.empty()) {
        auto *animation = cocos2d::Animation::createWithSpriteFrames(
            this->explosionFrames, 0.05f);
        auto *animate = cocos2d::Animate::create(animation);
        auto callback = cocos2d::CallFunc::create([this]() {
            this->bullet->removeFromParent();
            this->bullet = nullptr;
        });
        auto seq = cocos2d::Sequence::create(animate, callback, nullptr);
        this->bullet->setScale(0.5f);
        this->bullet->setRotation(0);
        this->bullet->setTexture("images/bullet/bomb/bomb00.png");
        this->bullet->runAction(seq);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
            "audio/bomb.MP3");
    } else {
        auto particle =
            cocos2d::ParticleSystemQuad::create("particles/blood.plist");
        if (particle) {
            particle->setScale(0.2f);
            particle->setPosition(this->enemySprite->getPositionX(),
                                  this->enemySprite->getPositionY());
        }
        this->levelScene->addChild(particle, 4);
        this->bullet->removeFromParent();
        this->bullet = nullptr;
    }
}

void Bullet::move() {
    if (this->isExploding) {
        return;
    }
    if (this->enemySprite == nullptr) {
        this->bullet->removeFromParent();
        this->bullet = nullptr;
        return;
    }
    if (this->isTouch()) {
        this->explosion();
        return;
    }
    this->updateAngle();
    this->bullet->setRotation(this->angle);
    this->bullet->setPositionX(this->bullet->getPositionX() + this->dx);
    this->bullet->setPositionY(this->bullet->getPositionY() + this->dy);
}

void Bullet::updateAngle() {
    float x = this->bullet->getPositionX();
    float y = this->bullet->getPositionY();
    float enemyX = this->enemySprite->getPositionX();
    float enemyY = this->enemySprite->getPositionY();
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
        this->angle =
            (float)(atan((y - enemyY) / (enemyX - x)) * 180.0f / M_PI);
    }
    if (enemyX < x) {
        this->angle += 180.0f;
    }
}

bool Bullet::isTouch() {
    if (this->enemySprite == nullptr) {
        return true;
    }
    float enemyX = this->enemySprite->getPositionX();
    float enemyY = this->enemySprite->getPositionY();
    float bulletX = this->bullet->getPositionX();
    float bulletY = this->bullet->getPositionY();
    float distance = sqrt(pow(enemyX - bulletX, 2) + pow(enemyY - bulletY, 2));
    if (distance < 50.0f) {
        return true;
    }
    return false;
}

/* ---------------------- MagicBullet ------------------------ */

MagicBullet::MagicBullet(LevelScene *levelScene,
                         towerdefence::core::Tower *tower,
                         towerdefence::core::Enemy *enemy)
    : Bullet(levelScene, tower, enemy) {
    switch (tower->status().tower_type_) {
    case TowerType::MagicianBase:
    case TowerType::DecelerateMagician:
    case TowerType::DecelerateMagicianPlus:
        this->color = "blue";
        break;
    case TowerType::CoreMagician:
    case TowerType::CoreMagicianPlus:
    case TowerType::WeakenMagician:
    case TowerType::WeakenMagicianPlus:
        this->color = "purple";
        break;
    case TowerType::DiffusiveMagician:
    case TowerType::DiffusiveMagicianPlus:
    case TowerType::HelperBase:
        this->color = "green";
        break;
    case TowerType::SpecialMagician:
    case TowerType::SpecialMagicianPlus:
    case TowerType::AggressiveMagician:
    case TowerType::AggressiveMagicianPlus:
        this->color = "orange";
        break;
    default:
        this->color = "blue";
        break;
    }
    std::string prefix = "images/bullet/" + this->color + "Bomb/" + this->color;
    this->explosionFrames.reserve(20);
    for (int i = 0; i < 20; i++) {
        std::string frameName = std::format("Bomb{:02d}.png", i);
        auto *frame = cocos2d::SpriteFrame::create(
            prefix + frameName, cocos2d::Rect(0, 0, 1270, 1270));
        this->explosionFrames.pushBack(frame);
    }
}

void MagicBullet::move() {
    if (this->isExploding) {
        return;
    }
    if (this->isTouch()) {
        this->explosion();
        return;
    }
    if (this->enemySprite == nullptr) {
        this->bullet->removeFromParent();
        this->bullet = nullptr;
        return;
    }
    Bullet::move();
    std::string prefix =
        "images/bullet/" + this->color + "Bullet/" + this->color;
    std::string bulletName =
        std::format("Bullet{:02d}.png", this->currentFrame);
    this->bullet->setTexture(prefix + bulletName);
    this->currentFrame = (this->currentFrame + 1) % this->totalFrames;
}

void MagicBullet::explosion() {
    this->isExploding = true;
    if (this->enemySprite == nullptr) {
        this->bullet->removeFromParent();
        this->bullet = nullptr;
        return;
    }
    this->bullet->setRotation(0);
    if (this->isScaling) {
        this->bullet->setScale(0.5f);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
            "audio/bomb.MP3");
    } else {
        this->bullet->setScale(0.15f);
    }
    std::string prefix = "images/bullet/" + this->color + "Bomb/" + this->color;
    this->bullet->setTexture(prefix + "Bomb00.png");
    this->bullet->setPosition(this->enemySprite->getPositionX(),
                              this->enemySprite->getPositionY());

    auto *animation = cocos2d::Animation::createWithSpriteFrames(
        this->explosionFrames, 0.05f);
    auto *animate = cocos2d::Animate::create(animation);
    auto callback = cocos2d::CallFunc::create([this]() {
        this->bullet->removeFromParent();
        this->bullet = nullptr;
    });
    auto seq = cocos2d::Sequence::create(animate, callback, nullptr);
    this->bullet->runAction(seq);
}

/* ---------------------- TowerAnimation ---------------------- */

bool TowerAnimation::isOnEffect = false;

void TowerAnimation::releaseSkill(LevelScene *levelScene,
                                  towerdefence::core::Tower *tower,
                                  float duration) {
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    frames.reserve(48);
    for (int i = 0; i < 48; i++) {
        std::string skillPath =
            std::format("images/bullet/skill/skill{:02d}.png", i);
        auto *frame = cocos2d::SpriteFrame::create(
            skillPath, cocos2d::Rect(0, 0, 1080, 1080));
        frames.pushBack(frame);
    }
    auto *animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto *animate = cocos2d::Animate::create(animation);
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + size;
    float typeY = origin.y + visibleSize.height - size;
    auto towerSprite = levelScene->getTower(tower->id);
    float x = towerSprite->getPositionX();
    float y = towerSprite->getPositionY();
    int indexX = (int)((x - typeX + 0.5f * size) / size);
    int indexY = (int)((typeY - y + 0.5f * size) / size);

    auto *skillSprite =
        cocos2d::Sprite::create("images/bullet/skill/skill00.png");
    switch (tower->status().tower_type_) {
    case TowerType::HighspeedArcher:
    case TowerType::HighspeedArcherPlus:
    case TowerType::CoreMagician:
        if (abs(duration - 1.0f) <= epsilon) {
            tower->set_storage<bool>("isScaling", true);
        }
        return;
    case TowerType::BomberPlus:
        skillSprite->scheduleOnce(
            [skillSprite, duration, indexX, indexY, levelScene, typeX,
             typeY](float dt) {
                skillSprite->scheduleOnce(
                    [skillSprite](float dt) {
                        skillSprite->removeFromParent();
                    },
                    duration - 3.0f,
                    "remove" + std::to_string(TowerAnimation::removeCounter++));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    "audio/bomberPlus.MP3");
                for (int i = indexY - 3; i <= indexY + 3; i++) {
                    for (int j = indexX - 3; j <= indexX + 3; j++) {
                        if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                            auto &grid = levelScene->map->get_ref(i, j).grid;
                            if (grid.type == Grid::Type::BlockPath &&
                                isInRange(i, j, indexY, indexX, 3)) {
                                auto particle =
                                    cocos2d::ParticleSystemQuad::create(
                                        "particles/explosion.plist");
                                particle->setPosition(cocos2d::Vec2(
                                    typeX + size * j, typeY - size * i - 70));
                                particle->setScale(0.25f);
                                levelScene->addChild(particle, 4);
                            }
                        }
                    }
                }
            },
            3.0f, "wait" + std::to_string(TowerAnimation::removeCounter++));
        break;
    case TowerType::Bomber:
    case TowerType::SpecialMagician:
        if (abs(duration - 1.0f) <= epsilon) {
            tower->set_storage<bool>("isScaling", true);
        }
        return;
    case TowerType::ArcherBase:
    case TowerType::Archer:
    case TowerType::ArcherPlus:
    case TowerType::MagicianBase:
    case TowerType::CoreMagicianPlus:
    case TowerType::DiffusiveMagician:
    case TowerType::SpecialMagicianPlus:
    case TowerType::HelperBase:
    case TowerType::DecelerateMagician:
        skillSprite->scheduleOnce(
            [skillSprite](float dt) { skillSprite->removeFromParent(); },
            duration,
            "remove" + std::to_string(TowerAnimation::removeCounter++));
        break;
    case TowerType::DiffusiveMagicianPlus:
        skillSprite->scheduleOnce(
            [skillSprite](float dt) { skillSprite->removeFromParent(); },
            duration,
            "remove" + std::to_string(TowerAnimation::removeCounter++));
        for (int i = indexY - 3; i <= indexY + 3; i++) {
            for (int j = indexX - 3; j <= indexX + 3; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (grid.type == Grid::Type::BlockPath &&
                        isInRange(i, j, indexY, indexX, 3)) {
                        auto particle = cocos2d::ParticleSystemQuad::create(
                            "particles/fire.plist");
                        particle->setPosition(cocos2d::Vec2(
                            typeX + size * j, typeY - size * i - 70));
                        particle->setScale(0.25f);
                        levelScene->addChild(particle, 4);
                        particle->scheduleOnce(
                            [particle](float dt) {
                                particle->removeFromParent();
                            },
                            duration,
                            "removeFire" +
                                std::to_string(
                                    TowerAnimation::removeCounter++));
                    }
                }
            }
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
            "audio/fire.MP3", false);
        break;
    case TowerType::WeakenMagician:
    case TowerType::WeakenMagicianPlus:
        skillSprite->scheduleOnce(
            [skillSprite](float dt) { skillSprite->removeFromParent(); },
            duration,
            "remove" + std::to_string(TowerAnimation::removeCounter++));
        for (int i = indexY - 3; i <= indexY + 3; i++) {
            for (int j = indexX - 3; j <= indexX + 3; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (grid.type == Grid::Type::BlockPath &&
                        isInRange(i, j, indexY, indexX, 3)) {
                        auto particle = cocos2d::ParticleSystemQuad::create(
                            "particles/weaken.plist");
                        particle->setPosition(
                            cocos2d::Vec2(typeX + size * j, typeY - size * i));
                        particle->setScale(0.25f);
                        levelScene->addChild(particle, 4);
                        particle->scheduleOnce(
                            [particle](float dt) {
                                particle->removeFromParent();
                            },
                            duration,
                            "removeWeaken" +
                                std::to_string(
                                    TowerAnimation::removeCounter++));
                    }
                }
            }
        }
        break;
    case TowerType::DecelerateMagicianPlus:
        skillSprite->scheduleOnce(
            [skillSprite](float dt) { skillSprite->removeFromParent(); },
            duration,
            "remove" + std::to_string(TowerAnimation::removeCounter++));
        for (int i = indexY - 3; i <= indexY + 3; i++) {
            for (int j = indexX - 3; j <= indexX + 3; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (grid.type == Grid::Type::BlockPath &&
                        isInRange(i, j, indexY, indexX, 3)) {
                        auto particle = cocos2d::ParticleSystemQuad::create(
                            "particles/decelerate.plist");
                        particle->setPosition(
                            cocos2d::Vec2(typeX + size * j, typeY - size * i));
                        particle->setScale(0.25f);
                        levelScene->addChild(particle, 4);
                        particle->scheduleOnce(
                            [particle](float dt) {
                                particle->removeFromParent();
                            },
                            duration,
                            "removeDecelerate" +
                                std::to_string(
                                    TowerAnimation::removeCounter++));
                    }
                }
            }
        }
        break;
    case TowerType::AggressiveMagician:
        for (int i = indexY - 3; i <= indexY + 3; i++) {
            for (int j = indexX - 3; j <= indexX + 3; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (grid.type == Grid::Type::BlockTower &&
                        grid.tower.has_value() &&
                        isInRange(i, j, indexY, indexX, 3) &&
                        (i != indexY || j != indexX)) {
                        auto attackUp = cocos2d::Sprite::create(
                            "images/towers/attack_up.png");
                        attackUp->setPosition(cocos2d::Vec2(
                            typeX + size * j + 45, typeY - size * i));
                        levelScene->addChild(attackUp, 4);
                    }
                }
            }
        }
        if (!TowerAnimation::isOnEffect) {
            TowerAnimation::isOnEffect = true;
        } else {
            return;
        }
        break;
    case TowerType::AggressiveMagicianPlus:
        for (int i = indexY - 3; i <= indexY + 3; i++) {
            for (int j = indexX - 3; j <= indexX + 3; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (grid.type == Grid::Type::BlockTower &&
                        grid.tower.has_value() &&
                        isInRange(i, j, indexY, indexX, 3) &&
                        (i != indexY || j != indexX)) {
                        auto attackUp = cocos2d::Sprite::create(
                            "images/towers/attack_up.png");
                        attackUp->setPosition(cocos2d::Vec2(
                            typeX + size * j + 45, typeY - size * i));
                        levelScene->addChild(attackUp, 4);
                        attackUp->scheduleOnce(
                            [attackUp](float dt) {
                                attackUp->removeFromParent();
                            },
                            duration,
                            "removeAttackUp" +
                                std::to_string(
                                    TowerAnimation::removeCounter++));
                    }
                }
            }
        }
        skillSprite->scheduleOnce(
            [skillSprite](float dt) { skillSprite->removeFromParent(); },
            duration,
            "remove" + std::to_string(TowerAnimation::removeCounter++));
        break;
    default:
        break;
    }
    skillSprite->setPosition(towerSprite->getPosition());
    skillSprite->setScale(0.15f);
    skillSprite->setOpacity(100);
    levelScene->addChild(skillSprite, 4);
    skillSprite->runAction(cocos2d::RepeatForever::create(animate));
}
