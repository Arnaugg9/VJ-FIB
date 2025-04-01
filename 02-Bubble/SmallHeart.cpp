#include <iostream>
#include <GL/glew.h>
#include "SmallHeart.h"
#include "Game.h"
#include "Player.h"

SmallHeart::SmallHeart()
{
}

SmallHeart::~SmallHeart()
{
}

void SmallHeart::init(const glm::ivec2& heartPos, ShaderProgram& shaderProgram)
{
    heartColliderSize = glm::ivec2(8, 8);

    spritesheet.loadFromFile("images/item_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));
    sprite->changeAnimation(0);

    //sprite->setPosition(heartPos);
}

void SmallHeart::update(int deltaTime)
{
    sprite->update(deltaTime);
}

void SmallHeart::render()
{
    sprite->render();
}

void SmallHeart::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void SmallHeart::setPosition(const glm::vec2& pos)
{
    posHeart = pos;
    posHeartCollision = glm::ivec2(posHeart.x + 4, posHeart.y + 8);
    sprite->setPosition(posHeart);
}

glm::ivec2 SmallHeart::getPosition()
{
    return posHeart;
}

glm::ivec2 SmallHeart::getColliderPosition()
{
    return posHeartCollision;
}

glm::ivec2 SmallHeart::getColliderSize()
{
    return heartColliderSize;
}