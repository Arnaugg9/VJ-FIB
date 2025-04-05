#include <iostream>
#include <GL/glew.h>
#include "Gourd.h"
#include "Game.h"

Gourd::Gourd()
{
}

Gourd::~Gourd()
{
}

void Gourd::init(const glm::ivec2& gourdPos, ShaderProgram& shaderProgram)
{
    active = true;
    onScreen = false;

    gourdColliderSize = glm::ivec2(8, 15);

    spritesheet.loadFromFile("images/item_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.66f, 0.0f));
    sprite->changeAnimation(0);

    setPosition(gourdPos);
}

void Gourd::update(int deltaTime)
{
    setPosition(glm::ivec2(posGourd.x, posGourd.y + 1));
    map->collisionMoveDown(posGourdCollision, gourdColliderSize, &posGourd.y);
    setPosition(posGourd);
    sprite->update(deltaTime);
}

void Gourd::render()
{
    sprite->render();
}

void Gourd::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Gourd::setPosition(const glm::vec2& pos)
{
    posGourd = pos;
    posGourdCollision = glm::vec2(posGourd.x, posGourd.y+1);
    sprite->setPosition(posGourd);
}

glm::ivec2 Gourd::getPosition()
{
    return posGourd;
}

glm::ivec2 Gourd::getColliderPosition()
{
    return posGourdCollision;
}

glm::ivec2 Gourd::getColliderSize()
{
    return gourdColliderSize;
}

bool Gourd::isOnScreen()
{
    return active && onScreen;
}

void Gourd::setOnScreen(bool value)
{
    onScreen = value;
}