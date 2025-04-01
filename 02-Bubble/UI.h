#ifndef UI_INCLUDE
#define UI_INCLUDE

#include "Texture.h"
#include "TileMap.h"
#include "Sprite.h"

class UI
{
public:
    UI();
    ~UI();

    void init(const glm::ivec2& uiPos, ShaderProgram& shaderProgram);
    void update(int deltaTime, const glm::vec2& cameraPos, bool spear);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

private:
    glm::ivec2 posUI;
    glm::vec2 uiOffset;
    Texture spritesheet;
    Sprite* spear;
	Sprite* fire;
	bool spearActive;
    Sprite* hearts[12];
    Sprite* helmet;
    Sprite* rock;
    TileMap* map;
	ShaderProgram* shaderProgram;
};

#endif // UI_INCLUDE