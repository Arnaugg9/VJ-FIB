#ifndef _HELMET_INCLUDE
#define _HELMET_INCLUDE

#include "Texture.h"
#include "TileMap.h"
#include "Sprite.h"

class Helmet
{
public:
    Helmet();
    ~Helmet();

    void init(const glm::ivec2& helmetPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::ivec2 getPosition();
    glm::ivec2 getColliderPosition();
    glm::ivec2 getColliderSize();

private:
    glm::ivec2 helmetColliderSize;
    glm::ivec2 posHelmet;
    glm::ivec2 posHelmetCollision;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
};

#endif // _HELMET_INCLUDE