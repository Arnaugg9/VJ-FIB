#include <iostream>
#include <GL/glew.h>
#include "Helmet.h"
#include "Game.h"

Helmet::Helmet()
{
}

Helmet::~Helmet()
{
}

void Helmet::init(const glm::ivec2& helmetPos, ShaderProgram& shaderProgram)
{
    active = true;
    onScreen = false;

    helmetColliderSize = glm::ivec2(16, 16);

    spritesheet.loadFromFile("images/item_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.33333333333f));
    sprite->changeAnimation(0);

    setPosition(helmetPos);
}

void Helmet::update(int deltaTime)
{
    setPosition(glm::ivec2(posHelmet.x, posHelmet.y + 1));
    map->collisionMoveDown(posHelmetCollision, helmetColliderSize, &posHelmet.y);
    setPosition(posHelmet);

    sprite->update(deltaTime);
}

void Helmet::render()
{
    sprite->render();
}

void Helmet::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Helmet::setPosition(const glm::vec2& pos)
{
    posHelmet = pos;
    posHelmetCollision = posHelmet;
    sprite->setPosition(posHelmet);
}

glm::ivec2 Helmet::getPosition()
{
    return posHelmet;
}

glm::ivec2 Helmet::getColliderPosition()
{
    return posHelmetCollision;
}

glm::ivec2 Helmet::getColliderSize()
{
    return helmetColliderSize;
}

bool Helmet::isOnScreen()
{
    return active && onScreen;
}

void Helmet::setOnScreen(bool value)
{
    onScreen = value;
}