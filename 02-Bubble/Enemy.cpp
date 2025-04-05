#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enemy.h"
#include "Game.h"

#define TIME_INVENCIBILITY 1000
#define FRAMES_AUX_HURT_ANIMATION 5


void Enemy::init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram) {
    posEnemy = enemyPos;

    onScreen = false;
    hasBullet = false;

    invencible = false;
    auxAnimationHurt = FRAMES_AUX_HURT_ANIMATION;

    spritesheet.loadFromFile("images/enemiesSprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.0625), &spritesheet, &shaderProgram);
}

void Enemy::update(int deltaTime) {
    sprite->update(deltaTime);
    if (invencible) {
        timeInvencibility -= deltaTime;
        if (timeInvencibility < 0) {
            invencible = false;
            timeInvencibility = TIME_INVENCIBILITY;
        }
    }
    sprite->setPosition(posEnemy);
}

void Enemy::render() {
    if (invencible && timeInvencibility > 0) {
        if (auxAnimationHurt <= FRAMES_AUX_HURT_ANIMATION / 2) sprite->render();
        if (auxAnimationHurt <= 0) {
            auxAnimationHurt = FRAMES_AUX_HURT_ANIMATION;
        }
        --auxAnimationHurt;
    }
    else {
        sprite->render();
    }
}

void Enemy::setPosition(const glm::vec2& pos) {
    posEnemy = pos;
    sprite->setPosition(posEnemy);
}

glm::ivec2 Enemy::getPosition() const {
    return posEnemy; 
}

glm::ivec2 Enemy::getSize() const {
    return sizeEnemy; 
}

glm::ivec2 Enemy::getColliderPosition() const {
    return posCollider; 
}

glm::ivec2 Enemy::getColliderSize() const {
    return sizeCollider; 
}

bool Enemy::isOnScreen() const {
    return onScreen; 
}

void Enemy::setOnScreen(bool state) {
    onScreen = state; 
}

bool Enemy::getHasBullet()
{
    return hasBullet;
}

bool Enemy::getHurt(int damage) {
    health -= damage;
    invencible = true;
    if (health <= 0) return true;
    return false;
}

int Enemy::getDamage() {
    return damage;
}

void Enemy::setMap(TileMap* tileMap)
{
    map = tileMap;
}

Bullet* Enemy::getBullet()
{
    return nullptr;
}

bool Enemy::isInvencible()
{
    return invencible;
}


