#ifndef _SMALLHEART_INCLUDE
#define _SMALLHEART_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class SmallHeart
{
public:
    SmallHeart();
    ~SmallHeart();

    void init(const glm::ivec2& heartPos, ShaderProgram& shaderProgram);
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
    glm::ivec2 heartColliderSize;
    glm::ivec2 posHeart;
    glm::ivec2 posHeartCollision;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;

    bool onScreen;
};

#endif // _SMALLHEART_INCLUDE