#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "EnemyFlower.h"
#include "EnemyElephant.h"
#include "EnemySnail.h"
#include "SmallHeart.h"
#include "BigHeart.h"
#include "Gourd.h"
#include "Armor.h"
#include "Helmet.h"
#include "UI.h"


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
	void updateEnemiesOnScreen(int deltaTime);
	void updateItems(int deltaTime);
	void updateEnemy(int deltaTime, Enemy* enemy);
	void renderEnemiesOnScreen();
	void renderItems();
	bool checkIfOnScreen(glm::ivec2 pos, glm::ivec2 size);
	bool collidesWithPlayer(glm::ivec2 posColliderEnemy, glm::ivec2 sizeColliderEnemy);
	void updateScreen();


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

	UI* ui;

	//Llistat d'objectes
	SmallHeart* smallHeart;
	BigHeart* bigHeart;
	Gourd* gourd[2];
	Armor* armor;
	Helmet* helmet;

	//Llistat d'enemics
	EnemyFlower *enemyFlower[5];
	EnemyElephant* enemyElephant[7];
	EnemySnail* enemySnail[9];

};


#endif // _SCENE_INCLUDE

