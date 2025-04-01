#ifndef ARMOR_H
#define ARMOR_H

#include "Texture.h"
#include "TileMap.h"
#include "Sprite.h"

class Armor
{
public:
    Armor();
    ~Armor();

    void init(const glm::ivec2& armorPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::ivec2 getPosition();
    glm::ivec2 getColliderPosition();
    glm::ivec2 getColliderSize();

private:
    glm::ivec2 armorColliderSize;
    glm::ivec2 posArmor;
    glm::ivec2 posArmorCollision;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
};

#endif // _ARMOR_INCLUDE