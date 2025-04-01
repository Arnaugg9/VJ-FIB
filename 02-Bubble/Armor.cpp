#include <iostream>
#include <GL/glew.h>
#include "Armor.h"
#include "Game.h"

Armor::Armor()
{
}

Armor::~Armor()
{
}

void Armor::init(const glm::ivec2& armorPos, ShaderProgram& shaderProgram)
{
    armorColliderSize = glm::ivec2(16, 16);

    spritesheet.loadFromFile("images/item_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.333333333333f, 0.33333333333f));
    sprite->changeAnimation(0);

    //sprite->setPosition(armorPos);
}

void Armor::update(int deltaTime)
{
    sprite->update(deltaTime);
}

void Armor::render()
{
    sprite->render();
}

void Armor::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Armor::setPosition(const glm::vec2& pos)
{
    posArmor = pos;
    posArmorCollision = posArmor;
    sprite->setPosition(posArmor);
}

glm::ivec2 Armor::getPosition()
{
    return posArmor;
}

glm::ivec2 Armor::getColliderPosition()
{
    return posArmorCollision;
}

glm::ivec2 Armor::getColliderSize()
{
    return armorColliderSize;
}
