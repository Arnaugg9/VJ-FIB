#include <iostream>
#include <GL/glew.h>
#include "BigHeart.h"
#include "Game.h"

BigHeart::BigHeart()
{
}

BigHeart::~BigHeart()
{
}

void BigHeart::init(const glm::ivec2& heartPos, ShaderProgram& shaderProgram)
{
    heartColliderSize = glm::ivec2(16, 16);

    spritesheet.loadFromFile("images/item_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.333333333333f, 0.0f));
    sprite->changeAnimation(0);

    //sprite->setPosition(heartPos);
}

void BigHeart::update(int deltaTime)
{
    sprite->update(deltaTime);
}

void BigHeart::render()
{
    sprite->render();
}

void BigHeart::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void BigHeart::setPosition(const glm::vec2& pos)
{
    posHeart = pos;
    posHeartCollision = posHeart;
    sprite->setPosition(posHeart);
}

glm::ivec2 BigHeart::getPosition()
{
    return posHeart;
}

glm::ivec2 BigHeart::getColliderPosition()
{
    return posHeartCollision;
}

glm::ivec2 BigHeart::getColliderSize()
{
    return heartColliderSize;
}