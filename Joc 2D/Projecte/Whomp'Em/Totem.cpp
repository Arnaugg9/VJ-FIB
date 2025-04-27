#include <iostream>
#include <GL/glew.h>
#include "Totem.h"
#include "Game.h"
#include "Player.h"

Totem::Totem()
{
}

Totem::~Totem()
{
}

void Totem::init(ShaderProgram& shaderProgram)
{
    active = false;
    onScreen = false;

    totemColliderSize = glm::ivec2(16, 16);

    spritesheet.loadFromFile("images/item_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(2/3.f, 2/3.f));
    sprite->changeAnimation(0);
}

void Totem::update(int deltaTime)
{
    setPosition(glm::ivec2(posTotem.x, posTotem.y + 1));
    map->collisionMoveDown(posTotemCollision, totemColliderSize, &posTotem.y);
    setPosition(posTotem);

    sprite->update(deltaTime);
}

void Totem::render()
{
    sprite->render();
}

void Totem::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Totem::setPosition(const glm::vec2& pos)
{
    posTotem = pos;
    posTotemCollision = glm::ivec2(posTotem);
    sprite->setPosition(posTotem);
}

glm::ivec2 Totem::getPosition()
{
    return posTotem;
}

glm::ivec2 Totem::getColliderPosition()
{
    return posTotemCollision;
}

glm::ivec2 Totem::getColliderSize()
{
    return totemColliderSize;
}

bool Totem::isOnScreen()
{
    return active && onScreen;
}

void Totem::setOnScreen(bool value)
{
    onScreen = value;
}
