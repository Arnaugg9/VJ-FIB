#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "EnemyFlower.h"
#include "EnemyElephant.h"
#include "EnemySnail.h"


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

	//Player* getPlayer() { return player; };

private:
	void initShaders();
	void updateEnemiesOnScreen(int deltaTime);
	void updateEnemy(int deltaTime, Enemy* enemy);
	void renderEnemiesOnScreen();
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
	

	//Llistat d'enemics
	EnemyFlower *enemyFlower[5];
	EnemyElephant* enemyElephant[7];
	EnemySnail* enemySnail[1];

};


#endif // _SCENE_INCLUDE

