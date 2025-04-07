#ifndef _TOTEM_INCLUDE
#define _TOTEM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Totem
{
public:
    Totem();
    ~Totem();

    void init(ShaderProgram& shaderProgram);
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
    glm::ivec2 totemColliderSize;
    glm::ivec2 posTotem;
    glm::ivec2 posTotemCollision;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;

    bool onScreen;
};

#endif // _TOTEM_INLCUDE