//
// Created by Ignorant on 2024/10/12.
//

#include "EnemyAnimation.h"

size_t EnemyAnimation::transportCounter = 0;
size_t EnemyAnimation::attackDownCounter = 0;
size_t EnemyAnimation::notAttackedCounter = 0;

static bool isInRange(int x1, int y1, int x2, int y2, int range) {
    return abs(x1 - x2) + abs(y1 - y2) <= range;
}

void EnemyAnimation::move(LevelScene *levelScene,
                          towerdefence::core::Enemy *enemy,
                          std::pair<size_t, size_t> currentPos,
                          std::pair<size_t, size_t> targetPos) {
    std::string prefix = "images/enemies/";
    switch (enemy->status().enemy_type_) {
    case EnemyType::AttackDown:
        prefix += "attackDown/move/attackDown_move";
        break;
    case EnemyType::Boss1:
        prefix += "boss/stage1/move";
        break;
    case EnemyType::Boss2:
        prefix += "boss/stage2";
        break;
    case EnemyType::Crab:
        prefix += "crab/move/crab_move";
        break;
    case EnemyType::Destroyer:
        prefix += "destroyer/move/destroyer_move";
        break;
    case EnemyType::Dog:
        prefix += "dog/move/dog_move";
        break;
    case EnemyType::LifeUp:
        prefix += "lifeUp/move/lifeUp_move";
        break;
    case EnemyType::NotAttacked:
        prefix += "notAttacked/move/notAttacked_move";
        break;
    case EnemyType::Soldier:
        prefix += "soldier/move/soldier_move";
        break;
    case EnemyType::SpeedUp:
        prefix += "speedUp/move/speedUp_move";
        break;
    case EnemyType::Tank:
        prefix += "tank/move/tank_move";
        break;
    case EnemyType::Warlock:
        prefix += "warlock/move/warlock_move";
        break;
    case EnemyType::Worm:
        prefix += "worm/move/worm_move";
        break;
    default:
        break;
    }
    int currentFrame = enemy->get_storage<int>("current_frame");
    std::string movePath = std::format("{:02d}.png", currentFrame);
    enemy->set_storage<int>("current_frame", (currentFrame + 1) % enemy->status().total_frames_);
    Id id = enemy->id;
    auto enemySprite = levelScene->getEnemy(id);
    enemySprite->setTexture(prefix + movePath);

    if (abs((int)currentPos.first - (int)targetPos.first) +
            abs((int)currentPos.second - (int)targetPos.second) >
        1) {
        EnemyAnimation::transport(levelScene, enemy, currentPos, targetPos);
        return;
    }

    float delta = (float)enemy->status().speed_ / 10.0f * size / 30.0f;
    if (currentPos.first < targetPos.first) {
        enemySprite->setPositionY(enemySprite->getPositionY() - delta);
    } else if (currentPos.first > targetPos.first) {
        enemySprite->setPositionY(enemySprite->getPositionY() + delta);
    }
    if (currentPos.second < targetPos.second) {
        enemySprite->setPositionX(enemySprite->getPositionX() + delta);
        enemySprite->setFlippedX(false);
    } else if (currentPos.second > targetPos.second) {
        enemySprite->setPositionX(enemySprite->getPositionX() - delta);
        enemySprite->setFlippedX(true);
    }
}

void EnemyAnimation::transport(LevelScene *levelScene,
                               towerdefence::core::Enemy *enemy,
                               std::pair<size_t, size_t> currentPos,
                               std::pair<size_t, size_t> targetPos) {
    auto enemySprite = levelScene->getEnemy(enemy->id);
    float duration = 1.0f / ((float)enemy->status().speed_ / 10.0f) / 2.0f;
    auto scaleDown = cocos2d::ScaleTo::create(duration, 0.1f);
    auto scaleUp = cocos2d::ScaleTo::create(duration, 1.0f);
    enemySprite->runAction(scaleDown);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    float x = origin.x + 350 + size * targetPos.second;
    float y = origin.y + visibleSize.height - size * targetPos.first;

    enemySprite->scheduleOnce(
        [x, y, enemySprite, scaleUp](float dt) {
            enemySprite->setPosition(x, y);
            enemySprite->runAction(scaleUp);
        },
        duration,
        "transport" + std::to_string(EnemyAnimation::transportCounter++));
}

void EnemyAnimation::releaseSkill(LevelScene *levelScene,
                                  towerdefence::core::Enemy *enemy,
                                  float duration) {
    std::string prefix = "images/enemies/";
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + size;
    float typeY = origin.y + visibleSize.height - size;
    auto enemySprite = levelScene->getEnemy(enemy->id);
    float x = enemySprite->getPositionX();
    float y = enemySprite->getPositionY();
    int indexX = (int)((x - typeX + 0.5f * size) / size);
    int indexY = (int)((typeY - y + 0.5f * size) / size);

    cocos2d::ParticleSystemQuad *particle;
    int counter = 0;
    switch (enemy->status().enemy_type_) {
    case EnemyType::AttackDown:
        prefix += "attackDown/skill/attackDown_skill";
        frames.reserve(45);
        for (int i = 0; i < 45; i++) {
            std::string skillPath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + skillPath, cocos2d::Rect(0, 0, 850, 850)));
        }
        for (int i = indexY - 2; i <= indexY + 2; i++) {
            for (int j = indexX - 2; j <= indexX + 2; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (isInRange(indexX, indexY, j, i, 2) &&
                        grid.type == Grid::Type::BlockTower &&
                        grid.tower.has_value()) {
                        auto &tower = grid.tower.value();
                        auto towerSprite = levelScene->getTower(tower->id);
                        float towerX = towerSprite->getPositionX();
                        float towerY = towerSprite->getPositionY();
                        cocos2d::Sprite *attackDown = cocos2d::Sprite::create(
                            "images/towers/attack_down.png");
                        attackDown->setPosition(
                            cocos2d::Vec2(towerX - 40, towerY));
                        levelScene->addChild(attackDown, 4);
                        attackDown->scheduleOnce(
                            [attackDown](float dt) {
                                attackDown->removeFromParent();
                            },
                            duration,
                            "attackDown" + std::to_string(i) +
                                std::to_string(j));
                    }
                }
            }
        }
        particle =
            cocos2d::ParticleSystemQuad::create("particles/attack_ring.plist");
        if (particle) {
            particle->scheduleOnce(
                [levelScene, particle, x, y](float dt) {
                    particle->setPosition(cocos2d::Vec2(x, y));
                    levelScene->addChild(particle, 2);
                },
                0.6f,
                "attackRing" +
                    std::to_string(EnemyAnimation::attackDownCounter++));
        }
        break;
    case EnemyType::NotAttacked:
        enemySprite->setOpacity(50);
        enemySprite->scheduleOnce(
            [enemySprite](float dt) {
                if (enemySprite) {
                    enemySprite->setOpacity(255);
                }
            },
            duration,
            "notAttacked" +
                std::to_string(EnemyAnimation::notAttackedCounter++));
        return;
    case EnemyType::LifeUp:
        particle =
            cocos2d::ParticleSystemQuad::create("particles/green_ring.plist");
        if (particle) {
            particle->setPosition(cocos2d::Vec2(x, y));
            levelScene->addChild(particle, 2);
        }
        return;
    case EnemyType::Boss1:
        prefix += "boss/stage1/skill/boss1_skill";
        frames.reserve(50);
        for (int i = 0; i < 50; i++) {
            std::string skillPath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + skillPath, cocos2d::Rect(0, 0, 850, 850)));
        }
        for (const auto &tower : levelScene->towers) {
            auto towerSprite = tower.second;
            float towerX = towerSprite->getPositionX();
            float towerY = towerSprite->getPositionY();
            cocos2d::Sprite *decelerate =
                cocos2d::Sprite::create("images/towers/ice.png");
            decelerate->setPosition(cocos2d::Vec2(towerX - 40, towerY + 40));
            levelScene->addChild(decelerate, 4);
            decelerate->scheduleOnce(
                [decelerate](float dt) { decelerate->removeFromParent(); },
                duration, "decelerate" + std::to_string(counter++));
        }
        for (int i = indexY - 2; i <= indexY + 2; i++) {
            for (int j = indexX - 2; j <= indexX + 2; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (grid.type == Grid::Type::BlockTower &&
                        grid.tower.has_value() &&
                        isInRange(indexX, indexY, j, i, 2)) {
                        auto &tower = grid.tower.value();
                        auto towerSprite = levelScene->getTower(tower->id);
                        float towerX = towerSprite->getPositionX();
                        float towerY = towerSprite->getPositionY();
                        cocos2d::Sprite *silence = cocos2d::Sprite::create(
                            "images/towers/silence.png");
                        silence->setPosition(
                            cocos2d::Vec2(towerX - 40, towerY - 40));
                        levelScene->addChild(silence, 4);
                        silence->scheduleOnce(
                            [silence](float dt) {
                                silence->removeFromParent();
                            },
                            duration,
                            "silence" + std::to_string(i) + std::to_string(j));
                    }
                }
            }
        }
        break;
    case EnemyType::Boss2:
        prefix += "boss/stage2/skill/boss2_skill";
        frames.reserve(50);
        for (int i = 0; i < 50; i++) {
            std::string skillPath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + skillPath, cocos2d::Rect(0, 0, 850, 850)));
        }
        if (abs(duration - 10.0f) <= epsilon) {
            for (const auto &tower : levelScene->towers) {
                auto towerSprite = tower.second;
                float towerX = towerSprite->getPositionX();
                float towerY = towerSprite->getPositionY();
                cocos2d::Sprite *decelerate =
                    cocos2d::Sprite::create("images/towers/ice.png");
                decelerate->setPosition(
                    cocos2d::Vec2(towerX - 40, towerY + 40));
                levelScene->addChild(decelerate, 4);
                decelerate->scheduleOnce(
                    [decelerate](float dt) { decelerate->removeFromParent(); },
                    duration, "decelerate" + std::to_string(counter++));
            }
        } else if (abs(duration - 15.0f) <= epsilon) {
            for (int i = indexY - 2; i <= indexY + 2; i++) {
                for (int j = indexX - 2; j <= indexX + 2; j++) {
                    if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                        auto &grid = levelScene->map->get_ref(i, j).grid;
                        if (grid.type == Grid::Type::BlockTower &&
                            grid.tower.has_value() &&
                            isInRange(indexX, indexY, j, i, 2)) {
                            auto &tower = grid.tower.value();
                            auto towerSprite = levelScene->getTower(tower->id);
                            float towerX = towerSprite->getPositionX();
                            float towerY = towerSprite->getPositionY();
                            cocos2d::Sprite *silence = cocos2d::Sprite::create(
                                "images/towers/silence.png");
                            silence->setPosition(
                                cocos2d::Vec2(towerX - 40, towerY - 40));
                            levelScene->addChild(silence, 4);
                            silence->scheduleOnce(
                                [silence](float dt) {
                                    silence->removeFromParent();
                                },
                                duration,
                                "silence" + std::to_string(i) +
                                    std::to_string(j));
                        }
                    }
                }
            }
        } else if (duration <= epsilon && !levelScene->towers.empty()) {
            cocos2d::Sprite *nearestTower = nullptr;
            int distance = 1000000;
            Id nearestId = levelScene->towers.begin()->first;
            for (const auto &tower : levelScene->towers) {
                auto id = tower.first;
                auto towerSprite = tower.second;
                float towerX = towerSprite->getPositionX();
                float towerY = towerSprite->getPositionY();
                int towerIndexX = (int)((towerX - typeX + 0.5f * size) / size);
                int towerIndexY = (int)((typeY - towerY + 0.5f * size) / size);
                auto temp =
                    abs(indexX - towerIndexX) + abs(indexY - towerIndexY);
                if (temp < distance) {
                    nearestTower = towerSprite;
                    distance = temp;
                    nearestId = id;
                }
            }
            if (nearestTower) {
                nearestTower->removeFromParent();
                levelScene->map->withdraw_tower(nearestId);
            }
        }
        break;
    default:
        return;
    }
    auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto animate = cocos2d::Animate::create(animation);
    auto callback = cocos2d::CallFunc::create(
        [enemy]() { enemy->set_storage<size_t>("current_frame", 0); });
    auto seq = cocos2d::Sequence::create(animate, callback, nullptr);
    enemySprite->runAction(seq);
}

void EnemyAnimation::dead(LevelScene *levelScene,
                          towerdefence::core::Enemy *enemy) {
    std::string prefix = "images/enemies/";
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    float typeX = origin.x + 350 + size;
    float typeY = origin.y + visibleSize.height - size;
    auto enemySprite = levelScene->getEnemy(enemy->id);
    float x = enemySprite->getPositionX();
    float y = enemySprite->getPositionY();
    int indexX = (int)((x - typeX + 0.5f * size) / size);
    int indexY = (int)((typeY - y + 0.5f * size) / size);

    cocos2d::ParticleSystemQuad *particle;
    switch (enemy->status().enemy_type_) {
    case EnemyType::AttackDown:
        prefix += "attackDown/die/attackDown_die";
        frames.reserve(41);
        for (int i = 0; i < 41; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Boss1:
        prefix += "boss/stage1/die/boss1_die";
        frames.reserve(216);
        for (int i = 0; i < 216; i++) {
            std::string diePath = std::format("{:03d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Boss2:
        prefix += "boss/stage2/die/boss2_die";
        frames.reserve(216);
        for (int i = 0; i < 216; i++) {
            std::string diePath = std::format("{:03d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Crab:
        prefix += "crab/die/crab_die";
        frames.reserve(36);
        for (int i = 0; i < 36; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Destroyer:
        prefix += "destroyer/die/destroyer_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Dog:
        prefix += "dog/die/dog_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::LifeUp:
        prefix += "lifeUp/die/lifeUp_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::NotAttacked:
        prefix += "notAttacked/die/notAttacked_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Soldier:
        prefix += "soldier/die/soldier_die";
        frames.reserve(23);
        for (int i = 0; i < 23; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::SpeedUp:
        enemySprite->unschedule("updateParticle");
        prefix += "speedUp/die/speedUp_die";
        frames.reserve(28);
        for (int i = 0; i < 28; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Tank:
        prefix += "tank/die/tank_die";
        frames.reserve(27);
        for (int i = 0; i < 27; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Warlock:
        prefix += "warlock/die/warlock_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::Worm:
        prefix += "worm/die/worm_die";
        frames.reserve(21);
        for (int i = 0; i < 21; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        for (int i = indexY - 1; i <= indexY + 1; i++) {
            for (int j = indexX - 1; j <= indexX + 1; j++) {
                if (i >= 0 && i < 7 && j >= 0 && j < 12) {
                    auto &grid = levelScene->map->get_ref(i, j).grid;
                    if (grid.type == Grid::Type::BlockTower &&
                        grid.tower.has_value()) {
                        auto &tower = grid.tower.value();
                        auto towerSprite = levelScene->getTower(tower->id);
                        float towerX = towerSprite->getPositionX();
                        float towerY = towerSprite->getPositionY();
                        cocos2d::Sprite *decelerate =
                            cocos2d::Sprite::create("images/towers/ice.png");
                        decelerate->setPosition(
                            cocos2d::Vec2(towerX - 40, towerY + 40));
                        levelScene->addChild(decelerate, 4);
                        decelerate->scheduleOnce(
                            [decelerate](float dt) {
                                decelerate->removeFromParent();
                            },
                            10.0f,
                            "decelerate" + std::to_string(i) +
                                std::to_string(j));
                    }
                }
            }
        }
        particle =
            cocos2d::ParticleSystemQuad::create("particles/icy_ring.plist");
        if (particle) {
            particle->scheduleOnce(
                [levelScene, particle, x, y](float dt) {
                    particle->setPosition(cocos2d::Vec2(x, y));
                    levelScene->addChild(particle, 2);
                },
                0.4f, "icyRing");
        }
        break;
    default:
        return;
    }
    auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto animate = cocos2d::Animate::create(animation);
    auto delay = cocos2d::DelayTime::create(0.5f);
    auto fadeOut = cocos2d::FadeOut::create(1.0f);
    auto remove = cocos2d::RemoveSelf::create(true);
    auto seq =
        cocos2d::Sequence::create(animate, delay, fadeOut, remove, nullptr);
    enemySprite->runAction(seq);
}
