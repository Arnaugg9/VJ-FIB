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
	void setPlayerHealth(int health);
	void setPlayerMaxHealth(int health);
	void setPlayerDefensiveHits(int hits);
    void setPlayerAttackingHits(int hits);
    void setBossfight();
	void setBossHealth(int health);

private:
    glm::ivec2 posUI;
    glm::vec2 uiOffset;
    Texture spritesheet;
    Texture spritesheetBoss;
    Sprite* spear;
	Sprite* fire;

	bool spearActive;
	int playerHealth;
	int playerMaxHealth;
	int playerDefensiveHits;
	int playerAttackingHits;
	int num_hearts;
	int full_hearts;
    int last_heart;

    bool bossfight;
	int bossHealth;
	int full_heartsBoss;
	int last_heartBoss;

    Sprite* hearts[12];
    Sprite* bossHearts[11];
    Sprite* helmet;
    Sprite* rock;
    TileMap* map;
	ShaderProgram* shaderProgram;
};

#endif // UI_INCLUDE