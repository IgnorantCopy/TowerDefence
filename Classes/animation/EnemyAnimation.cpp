//
// Created by Ignorant on 2024/10/12.
//

#include "EnemyAnimation.h"

void EnemyAnimation::move(LevelScene *levelScene, towerdefence::core::Enemy *enemy,
                          std::pair<size_t, size_t> currentPos, std::pair<size_t, size_t> targetPos) {
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
    std::string movePath = std::format("{:02d}.png", enemy->status().current_frame_);
    Id id = enemy->id;
    auto enemySprite = levelScene->getEnemy(id);
    enemySprite->setTexture(prefix + movePath);
    
    if (abs((int) currentPos.first - (int) targetPos.first) + abs((int) currentPos.second - (int) targetPos.second) >
        1) {
        EnemyAnimation::transport(levelScene, enemy, currentPos, targetPos);
        return;
    }
    
    float delta = (float) enemy->status().speed_ / 10.0f * size / 30.0f;
    if (currentPos.first < targetPos.first) {
        enemySprite->setPositionY(enemySprite->getPositionY() - delta);
    } else if (currentPos.first > targetPos.first) {
        enemySprite->setPositionY(enemySprite->getPositionY() + delta);
    }
    if (currentPos.second < targetPos.second) {
        enemySprite->setPositionX(enemySprite->getPositionX() + delta);
    } else if (currentPos.second > targetPos.second) {
        enemySprite->setPositionX(enemySprite->getPositionX() - delta);
    }
}

void EnemyAnimation::transport(LevelScene *levelScene, towerdefence::core::Enemy *enemy,
                               std::pair<size_t, size_t> currentPos, std::pair<size_t, size_t> targetPos) {
    auto enemySprite = levelScene->getEnemy(enemy->id);
    float duration = 1.0f / ((float) enemy->status().speed_ / 10.0f) / 2.0f;
    auto scaleDown = cocos2d::ScaleTo::create(duration, 0.1f);
    auto scaleUp = cocos2d::ScaleTo::create(duration, 1.0f);
    auto seq = cocos2d::Sequence::create(scaleDown, scaleUp, nullptr);
    enemySprite->runAction(seq);
}

void EnemyAnimation::releaseSkill(LevelScene *levelScene, towerdefence::core::Enemy *enemy) {
    std::string prefix = "images/enemies/";
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    switch (enemy->status().enemy_type_) {
        case EnemyType::AttackDown:
            prefix += "attackDown/skill/attackDown_skill";
            frames.reserve(45);
            for (int i = 0; i < 45; i++) {
                std::string skillPath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + skillPath, cocos2d::Rect(0, 0, 850, 850)));
            }
            break;
        case EnemyType::Boss1:
            prefix += "boss/stage1/skill/boss1_skill";
            frames.reserve(50);
            for (int i = 0; i < 50; i++) {
                std::string skillPath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + skillPath, cocos2d::Rect(0, 0, 850, 850)));
            }
            break;
        case EnemyType::Boss2:
            prefix += "boss/stage2/skill/boss2_skill";
            frames.reserve(50);
            for (int i = 0; i < 50; i++) {
                std::string skillPath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + skillPath, cocos2d::Rect(0, 0, 850, 850)));
            }
            break;
        default:
            return;
    }
    auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto animate = cocos2d::Animate::create(animation);
    auto enemySprite = levelScene->getEnemy(enemy->id);
    enemySprite->runAction(animate);
}

void EnemyAnimation::dead(LevelScene *levelScene, towerdefence::core::Enemy *enemy) {
    std::string prefix = "images/enemies/";
    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    switch (enemy->status().enemy_type_) {
        case EnemyType::AttackDown:
            prefix += "attackDown/die/attackDown_die";
            frames.reserve(41);
            for (int i = 0; i < 41; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Boss1:
            prefix += "boss/stage1/die/boss1_die";
            frames.reserve(216);
            for (int i = 0; i < 216; i++) {
                std::string diePath = std::format("{:03d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Boss2:
            prefix += "boss/stage2/die/boss2_die";
            frames.reserve(216);
            for (int i = 0; i < 216; i++) {
                std::string diePath = std::format("{:03d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Crab:
            prefix += "crab/die/crab_die";
            frames.reserve(36);
            for (int i = 0; i < 36; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Destroyer:
            prefix += "destroyer/die/destroyer_die";
            frames.reserve(30);
            for (int i = 0; i < 30; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Dog:
            prefix += "dog/die/dog_die";
            frames.reserve(30);
            for (int i = 0; i < 30; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::LifeUp:
            prefix += "lifeUp/die/lifeUp_die";
            frames.reserve(30);
            for (int i = 0; i < 30; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::NotAttacked:
            prefix += "notAttacked/die/notAttacked_die";
            frames.reserve(30);
            for (int i = 0; i < 30; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Soldier:
            prefix += "soldier/die/soldier_die";
            frames.reserve(23);
            for (int i = 0; i < 23; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::SpeedUp:
            prefix += "speedUp/die/speedUp_die";
            frames.reserve(28);
            for (int i = 0; i < 28; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Tank:
            prefix += "tank/die/tank_die";
            frames.reserve(27);
            for (int i = 0; i < 27; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Warlock:
            prefix += "warlock/die/warlock_die";
            frames.reserve(30);
            for (int i = 0; i < 30; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        case EnemyType::Worm:
            prefix += "worm/die/worm_die";
            frames.reserve(21);
            for (int i = 0; i < 21; i++) {
                std::string diePath = std::format("{:02d}.png", i);
                frames.pushBack(cocos2d::SpriteFrame::create(prefix + diePath, cocos2d::Rect(0, 0, 800, 800)));
            }
            break;
        default:
            return;
    }
    auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.05f);
    auto animate = cocos2d::Animate::create(animation);
    auto enemySprite = levelScene->getEnemy(enemy->id);
    enemySprite->runAction(animate);
}
