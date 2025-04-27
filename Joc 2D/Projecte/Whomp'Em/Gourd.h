#ifndef _GOURD_INCLUDE
#define _GOURD_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Gourd
{
public:
    Gourd();
    ~Gourd();

    void init(const glm::ivec2& gourdPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::ivec2 getPosition();
    glm::ivec2 getColliderPosition();
    glm::ivec2 getColliderSize();

    bool isOnScreen();
    void setOnScreen(bool value);
    bool active;

private:
    glm::ivec2 gourdColliderSize;
    glm::ivec2 posGourd;
    glm::ivec2 posGourdCollision;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;

    bool onScreen;
};

#endif // _GOURD_INCLUDE