//
// Created by Ignorant on 2024/10/12.
//

#include "EnemyAnimation.h"
#include "core/entity/enemy/Boss-2.h"

using towerdefence::core::Boss2;

size_t EnemyAnimation::notAttackedCounter = 0;
std::unique_ptr<EnemyFactoryBase> EnemyAnimation::boss = nullptr;

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
        prefix += "boss/stage1/move/boss1_move";
        break;
    case EnemyType::Boss2:
        prefix += "boss/stage2/move/boss2_move";
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
    auto enemySprite = levelScene->getEnemy(enemy->id);

    if (abs((int)currentPos.first - (int)targetPos.first) +
            abs((int)currentPos.second - (int)targetPos.second) >
        1) {
        EnemyAnimation::transport(levelScene, enemy, currentPos, targetPos);
        return;
    }

    float delta = (float)enemy->status().speed_ / 10.0f * size / 30.0f;
    cocos2d::MoveBy *moveX = nullptr;
    cocos2d::MoveBy *moveY = nullptr;
    if (currentPos.first < targetPos.first) {
        moveY = cocos2d::MoveBy::create(1.0f / 30, cocos2d::Vec2(0, -delta));
    } else if (currentPos.first > targetPos.first) {
        moveY = cocos2d::MoveBy::create(1.0f / 30, cocos2d::Vec2(0, delta));
    } else {
        moveY = cocos2d::MoveBy::create(1.0f / 30, cocos2d::Vec2(0, 0));
    }
    if (currentPos.second < targetPos.second) {
        moveX = cocos2d::MoveBy::create(1.0f / 30, cocos2d::Vec2(delta, 0));
        enemySprite->setFlippedX(false);
    } else if (currentPos.second > targetPos.second) {
        moveX = cocos2d::MoveBy::create(1.0f / 30, cocos2d::Vec2(-delta, 0));
        enemySprite->setFlippedX(true);
    } else {
        moveX = cocos2d::MoveBy::create(1.0f / 30, cocos2d::Vec2(0, 0));
    }
    auto move = cocos2d::Spawn::create(moveX, moveY, nullptr);
    auto callback = cocos2d::CallFunc::create([enemy, enemySprite, prefix]() {
        int currentFrame = enemy->get_storage<int>("current_frame");
        std::string movePath = std::format("{:02d}.png", currentFrame);
        enemy->set_storage<int>("current_frame",
                                (currentFrame + 1) %
                                    enemy->status().total_frames_);
        enemySprite->setTexture(prefix + movePath);
    });
    auto seqBase = cocos2d::Sequence::create(move, callback, nullptr);
    int numOfSeqs = (int)(size / delta);
    auto seq = cocos2d::Repeat::create(seqBase, numOfSeqs);
    enemySprite->runAction(seq);
}

void EnemyAnimation::transport(LevelScene *levelScene,
                               towerdefence::core::Enemy *enemy,
                               std::pair<size_t, size_t> currentPos,
                               std::pair<size_t, size_t> targetPos) {
    auto enemySprite = levelScene->getEnemy(enemy->id);
    float duration = 1.0f / ((float)enemy->status().speed_ / 10.0f) / 4.0f;
    auto scaleDown = cocos2d::ScaleTo::create(duration, 0.1f);
    float scaleRate = 0.25f;
    switch (enemy->status().enemy_type_) {
    case EnemyType::Worm:
        scaleRate = 0.4f;
        break;
    case EnemyType::Dog:
        scaleRate = 0.2f;
        break;
    case EnemyType::Soldier:
        scaleRate = 0.4f;
        break;
    case EnemyType::Warlock:
        scaleRate = 0.4f;
        break;
    case EnemyType::Destroyer:
        scaleRate = 0.25f;
        break;
    case EnemyType::Tank:
        scaleRate = 0.4f;
        break;
    case EnemyType::Crab:
        scaleRate = 0.25f;
        break;
    case EnemyType::SpeedUp:
        scaleRate = 0.4f;
        break;
    case EnemyType::AttackDown:
        scaleRate = 0.25f;
        break;
    case EnemyType::LifeUp:
        scaleRate = 0.25f;
        break;
    case EnemyType::NotAttacked:
        scaleRate = 0.35f;
        break;
    case EnemyType::Boss1:
        scaleRate = 0.4f;
        break;
    case EnemyType::Boss2:
        scaleRate = 0.4f;
        break;
    default:
        break;
    }
    auto scaleUp = cocos2d::ScaleTo::create(duration, scaleRate);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    float x = origin.x + 350 + size * (targetPos.second + 1);
    float y = origin.y + visibleSize.height - size * (targetPos.first + 1);
    auto callback = cocos2d::CallFunc::create(
        [enemySprite, x, y]() { enemySprite->setPosition(x, y); });
    auto seq = cocos2d::Sequence::create(scaleDown, callback, scaleUp, nullptr);
    enemySprite->runAction(seq);
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
    if (enemySprite == nullptr) {
        return;
    }
    std::pair<size_t, size_t> currentPos = levelScene->getEnemyPath(enemy->id);
    if (currentPos.first == 100 && currentPos.second == 100) {
        return;
    }
    float x = enemySprite->getPositionX();
    float y = enemySprite->getPositionY();
    float delta_X = 0.0f;
    float delta_Y = 0.0f;
    float delta_x = 0.0f;
    float delta_y = 0.0f;
    float delayTime = 0.0f;
    int indexX = int(currentPos.first);
    int indexY = int(currentPos.second);
    for (int i = 0; i < enemy->route_.pos; i++) {
        indexX += enemy->route_.diffs[i].first;
        indexY += enemy->route_.diffs[i].second;
    }

    cocos2d::ParticleSystemQuad *particle;
    auto player = CocosDenshion::SimpleAudioEngine::getInstance();
    int counter = 0;
    switch (enemy->status().enemy_type_) {
    case EnemyType::AttackDown:
        delta_y = 34.0;
        delta_Y = 15.0;
        delayTime = 0.054;
        prefix += "attackDown/skill/attackDown_skill";
        frames.reserve(45);
        for (int i = 0; i < 45; i++) {
            std::string skillPath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + skillPath, cocos2d::Rect(0, 0, 1000, 1000)));
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
                            cocos2d::Vec2(towerX - 45, towerY));
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
            particle->setPosition(cocos2d::Vec2(typeX + float(indexY) * size,
                                                typeY - float(indexX) * size + delta_Y));
            levelScene->addChild(particle, 4);
        }
        player->playEffect("audio/enemySkill.MP3");
        break;
    case EnemyType::NotAttacked:
        enemySprite->setOpacity(100);
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
        player->playEffect("audio/enemySkill.MP3");
        return;
    case EnemyType::Boss1:
        delta_x = 20.0;
        delta_y = 40.0;
        delta_X = 18.0;
        delta_Y = 25.0;
        prefix += "boss/stage1/skill1/boss1_skill1";
        frames.reserve(90);
        for (int i = 0; i < 90; i++) {
            std::string skillPath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + skillPath, cocos2d::Rect(0, 0, 1000, 1000)));
        }
        for (const auto &tower : levelScene->towers) {
            auto towerSprite = tower.second;
            float towerX = towerSprite->getPositionX();
            float towerY = towerSprite->getPositionY();
            cocos2d::Sprite *decelerate =
                cocos2d::Sprite::create("images/towers/ice.png");
            decelerate->setPosition(cocos2d::Vec2(towerX - 45, towerY + 45));
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
                            cocos2d::Vec2(towerX - 45, towerY - 45));
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
        prefix += "boss/stage2/skill";
        if (abs(duration - 10.0f) <= epsilon) {
            prefix += "1/boss2_skill1";
            frames.reserve(90);
            for (int i = 0; i < 90; i++) {
                std::string skillPath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(
                    prefix + skillPath, cocos2d::Rect(0, 0, 1000, 1000)));
            }
            for (const auto &tower : levelScene->towers) {
                auto towerSprite = tower.second;
                float towerX = towerSprite->getPositionX();
                float towerY = towerSprite->getPositionY();
                cocos2d::Sprite *decelerate =
                    cocos2d::Sprite::create("images/towers/ice.png");
                decelerate->setPosition(
                    cocos2d::Vec2(towerX - 45, towerY + 45));
                levelScene->addChild(decelerate, 4);
                decelerate->scheduleOnce(
                    [decelerate](float dt) { decelerate->removeFromParent(); },
                    duration, "decelerate" + std::to_string(counter++));
            }
        } else if (abs(duration - 15.0f) <= epsilon) {
            prefix += "3/boss2_skill3";
            frames.reserve(110);
            for (int i = 0; i < 110; i++) {
                std::string skillPath = std::format("{:03d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(
                    prefix + skillPath, cocos2d::Rect(0, 0, 1000, 1000)));
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
                                cocos2d::Vec2(towerX - 45, towerY - 45));
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
            prefix += "2/boss2_skill2";
            frames.reserve(48);
            for (int i = 0; i < 48; i++) {
                std::string skillPath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(
                    prefix + skillPath, cocos2d::Rect(0, 0, 1000, 1000)));
            }
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
    enemySprite->setPosition(cocos2d::Vec2(typeX + float(indexY) * size + delta_X + delta_x,
                                           typeY - float(indexX) * size + delta_Y + delta_y));
    auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto animate = cocos2d::Animate::create(animation);
    auto delay = cocos2d::DelayTime::create(delayTime);
    auto callback = cocos2d::CallFunc::create(
        [enemy, enemySprite, typeX, typeY, indexX, indexY, delta_X, delta_Y]() {
            enemy->set_storage<int>("current_frame", 0);
            enemySprite->setPosition(cocos2d::Vec2(typeX + float(indexY) * size + delta_X,
                                                   typeY - float(indexX) * size + delta_Y));
        });
    auto seq = cocos2d::Sequence::create(animate, delay, callback, nullptr);
    enemySprite->stopAllActions();
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
    cocos2d::Sprite *enemySprite = nullptr;
    for (auto it = levelScene->enemies.begin();
         it != levelScene->enemies.end();) {
        if (it->first == enemy->id) {
            enemySprite = it->second;
            it = levelScene->enemies.erase(it);
            break;
        } else {
            it++;
        }
    }
    if (enemySprite == nullptr) {
        return;
    }
    enemySprite->setOpacity(255);
    float x = enemySprite->getPositionX();
    float y = enemySprite->getPositionY();
    float delta_x = 0.0f;
    float delta_y = 0.0f;
    std::pair<size_t, size_t> currentPos = levelScene->getEnemyPath(enemy->id);
    if (currentPos.first == 100 && currentPos.second == 100) {
        return;
    }
    int index_x = (int)((x - typeX + 0.5f * size) / size);
    int index_y = (int)((typeY - y + 0.5f * size) / size);
    int indexX = int(currentPos.first);
    int indexY = int(currentPos.second);
    for (int i = 0; i < enemy->route_.pos; i++) {
        indexX += enemy->route_.diffs[i].first;
        indexY += enemy->route_.diffs[i].second;
    }

    auto extra_storage =
        std::unordered_map<std::string, std::any>{{"current_frame", 0}};
    Route route = Route(enemy->route_.diffs);
    route.pos = enemy->route_.pos;

    cocos2d::ParticleSystemQuad *particle;
    switch (enemy->status().enemy_type_) {
    case EnemyType::AttackDown:
        prefix += "attackDown/die/attackDown_die";
        frames.reserve(41);
        for (int i = 0; i < 41; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 1000, 1000)));
        }
        break;
    case EnemyType::Boss1:
        delta_x = 13.0;
        prefix += "boss/stage1/die/boss1_die";
        frames.reserve(216);
        for (int i = 0; i < 216; i++) {
            std::string diePath = std::format("{:03d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 1000, 1000)));
        }
        EnemyAnimation::boss =
            std::make_unique<EnemyFactory<Boss2>>(route, extra_storage);
        break;
    case EnemyType::Boss2:
        prefix += "boss/stage2/die/boss2_die";
        frames.reserve(134);
        for (int i = 0; i < 134; i++) {
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
        delta_x = -40.0;
        delta_y = 17.0;
        prefix += "destroyer/die/destroyer_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 900, 900)));
        }
        break;
    case EnemyType::Dog:
        prefix += "dog/die/dog_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 850, 850)));
        }
        break;
    case EnemyType::LifeUp:
        delta_x = -5.0;
        prefix += "lifeUp/die/lifeUp_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
        }
        break;
    case EnemyType::NotAttacked:
        delta_x = 3.0;
        prefix += "notAttacked/die/notAttacked_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 450, 450)));
        }
        break;
    case EnemyType::Soldier:
        prefix += "soldier/die/soldier_die";
        frames.reserve(23);
        for (int i = 0; i < 23; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 600, 600)));
        }
        break;
    case EnemyType::SpeedUp:
        delta_y = -6.0;
        prefix += "speedUp/die/speedUp_die";
        frames.reserve(28);
        for (int i = 0; i < 28; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 600, 600)));
        }
        break;
    case EnemyType::Tank:
        delta_x = -10.0;
        delta_y = 10.0;
        prefix += "tank/die/tank_die";
        frames.reserve(27);
        for (int i = 0; i < 27; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 650, 650)));
        }
        break;
    case EnemyType::Warlock:
        delta_y = -27.0;
        prefix += "warlock/die/warlock_die";
        frames.reserve(30);
        for (int i = 0; i < 30; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 500, 500)));
        }
        break;
    case EnemyType::Worm:
        prefix += "worm/die/worm_die";
        frames.reserve(21);
        for (int i = 0; i < 21; i++) {
            std::string diePath = std::format("{:02d}.png", i);
            frames.pushBack(cocos2d::SpriteFrame::create(
                prefix + diePath, cocos2d::Rect(0, 0, 400, 400)));
        }
        for (int i = index_y - 1; i <= index_y + 1; i++) {
            for (int j = index_x - 1; j <= index_x + 1; j++) {
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
                            cocos2d::Vec2(towerX - 45, towerY + 45));
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
    auto remove = cocos2d::RemoveSelf::create();
    cocos2d::Sequence *seq;
    if (enemy->status().enemy_type_ == EnemyType::Boss1) {
        auto callback = cocos2d::CallFunc::create([levelScene, indexX, indexY, typeX, typeY]() {
            if (EnemyAnimation::boss) {
                Id id = levelScene->map->spawn_enemy_at(indexY, indexX,
                                                        *EnemyAnimation::boss);
                auto newEnemySprite = cocos2d::Sprite::create(
                    "images/enemies/boss/stage2/move/boss2_move00.png");
                newEnemySprite->setPosition(typeX + float(indexY * size) + 10.0f, typeY - float(indexX * size) + 63.0f);
                newEnemySprite->setScale(0.45f);
                newEnemySprite->setOpacity(100);
                levelScene->enemies.emplace_back(id, newEnemySprite);
                levelScene->addChild(newEnemySprite, 5);
                newEnemySprite->scheduleOnce(
                    [newEnemySprite](float dt) { newEnemySprite->setOpacity(255); },
                    5.0f, "boss2");
            }
        });
        seq = cocos2d::Sequence::create(animate, delay, callback, remove,
                                        nullptr);
    } else {
        seq =
            cocos2d::Sequence::create(animate, delay, fadeOut, remove, nullptr);
    }
    enemySprite->setPosition(x + delta_x, y + delta_y);
    enemySprite->stopAllActions();
    enemySprite->runAction(seq);
}
