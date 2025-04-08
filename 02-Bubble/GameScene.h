#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "EnemyFlower.h"
#include "EnemyElephant.h"
#include "EnemySnail.h"
#include "Boss.h"
#include "SmallHeart.h"
#include "BigHeart.h"
#include "Gourd.h"
#include "Armor.h"
#include "Helmet.h"
#include "Totem.h"
#include "UI.h"
#include "SoundManager.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class GameScene
{

public:
	GameScene();
	~GameScene();

	void init();
	void update(int deltaTime);
	void render();

	void handleKeyPress(int key);

	//Player* getPlayer() { return player; };

private:
	void initShaders();
	void handleCamera();
	void handleScreenShake();
	void updateEnemiesOnScreen(int deltaTime);
	void updateItems(int deltaTime);
	void updateEnemy(int deltaTime, Enemy* enemy);
	void updateUI(int deltaTime);
	void renderEnemiesOnScreen();
	void renderItems();
	bool checkIfOnScreen(glm::ivec2 pos, glm::ivec2 size);
	bool collidesWithPlayer(glm::ivec2 posColliderEnemy, glm::ivec2 sizeColliderEnemy);
	bool collidesWithPlayerItem(glm::ivec2 posColliderEnemy, glm::ivec2 sizeColliderEnemy);
	void updateScreen();

	void spawnItem(glm::ivec2 pos, glm::ivec2 sizeEnemy);


private:
	TileMap *map;
	TileMap *background;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float leftCam;
	float topCam;
	bool verticalScroll;
	bool spear;

	bool godModeOn;

	bool bossScreenShake;
	bool bossDying;
	bool bossDead;
	int timerScreenShake;
	bool screenShakeDown;

	int totemUpTimer;
	bool endAnimation;
	int endAnimationTimer;

	bool paused;
	Texture pauseSpritesheet;
	Sprite* pauseScreenSprite;

	Texture dieSpritesheet;
	Sprite* dieScreenSprite;

	UI* ui;

	//Llistat d'objectes
	std::vector<SmallHeart*> smallHeart;
	std::vector<BigHeart*> bigHeart;
	std::vector<Gourd*> gourd;
	std::vector<Armor*> armor;
	std::vector<Helmet*> helmet;
	Totem* totem;

	//Llistat d'enemics
	EnemyFlower *enemyFlower[5];
	EnemyElephant* enemyElephant[7];
	EnemySnail* enemySnail[9];
	Boss* boss;

	bool music = false;
	bool musicTotem = false;
};


#endif // _SCENE_INCLUDE

