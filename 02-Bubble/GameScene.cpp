#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "GameScene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 39			//Original 33
#define INIT_PLAYER_Y_TILES 7			//Original 39

#define INIT_CAMERA_X 32*16				//Original 32*16
#define INIT_CAMERA_Y 0					//Original 30*16

#define TOP_HORIZONTAL_MIDDLE 30*16
#define LEFT_VERTICAL_RIGHT 128*16
#define TOP_IN_BOTTOM_OF_MAP 60*16
#define LEFT_VERTICAL_LEFT 0
#define TOP_IN_TOP_OF_MAP 0
#define LEFT_BOSSFIGHT 48*16

#define TIME_SCREEN_SHAKE 2000

#define TOTEM_UP_TIMER 1000
#define END_ANIMATION_DURATION 2000

#define SCREEN_MARGIN_UPDATE 90

#define NUM_ITEMS 5
#define ITEM_GENERATION_PROBABILITY 100

enum Directions
{
	LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, LEFT_UPSIDE, RIGHT_UPSIDE
};

enum ItemsList
{
	SMALL_HEART, BIG_HEART, ARMOR, GOURD, HELMET, 
};

enum Weapons{
	SPEAR, FIRE
};

GameScene::GameScene()
{
	map = NULL;
	player = NULL;
}

GameScene::~GameScene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void GameScene::init()
{
	//init mapa
	initShaders();
	background = TileMap::createTileMap("levels/background.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/level.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	godModeOn = false;
	paused = false;

	bossScreenShake = false;
	bossDying = false;
	bossDead = false;
	timerScreenShake = TIME_SCREEN_SHAKE;
	screenShakeDown = true;

	totemUpTimer = TOTEM_UP_TIMER;
	endAnimation = false;
	endAnimationTimer = END_ANIMATION_DURATION;

	//init player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	//init camara
	leftCam = INIT_CAMERA_X;
	topCam = INIT_CAMERA_Y;
	verticalScroll = false;
	projection = glm::ortho(leftCam, leftCam + 16*16, topCam + 15*16, topCam);

	//init time system
	currentTime = 0.0f;

	spear = true;

	//Init UI
	ui = new UI();
	ui->init(glm::vec2(32 * 16, 30 * 16), texProgram);

	//Init Pause screen
	pauseSpritesheet.loadFromFile("images/PauseMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pauseSpritesheet.setMinFilter(GL_NEAREST);
	pauseSpritesheet.setMagFilter(GL_NEAREST);

	pauseScreenSprite = Sprite::createSprite(glm::ivec2(256, 224), glm::vec2(1.f, 1.f), &pauseSpritesheet, &texProgram);
	pauseScreenSprite->setNumberAnimations(1);

	pauseScreenSprite->setAnimationSpeed(0, 8);
	pauseScreenSprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	pauseScreenSprite->setPosition(glm::ivec2(leftCam, topCam));
	pauseScreenSprite->changeAnimation(0);


	//Init enemics
		//Flowers
		enemyFlower[0] = new EnemyFlower();
		enemyFlower[0]->init(glm::vec2(83*16, 40*16), texProgram);

		enemyFlower[1] = new EnemyFlower();
		enemyFlower[1]->init(glm::vec2(99*16, 40*16), texProgram);

		enemyFlower[2] = new EnemyFlower();
		enemyFlower[2]->init(glm::vec2(106 * 16, 38 * 16), texProgram);

		enemyFlower[3] = new EnemyFlower();
		enemyFlower[3]->init(glm::vec2(113 * 16, 40 * 16), texProgram);

		enemyFlower[4] = new EnemyFlower();
		enemyFlower[4]->init(glm::vec2(124 * 16, 39 * 16), texProgram);

		
		//Elephants
		enemyElephant[0] = new EnemyElephant();
		enemyElephant[0]->init(glm::ivec2(58*16, 33*16), texProgram);
		enemyElephant[0]->setMap(map);

		enemyElephant[1] = new EnemyElephant();
		enemyElephant[1]->init(glm::ivec2(74*16, 36*16), texProgram);
		enemyElephant[1]->setMap(map);

		enemyElephant[2] = new EnemyElephant();
		enemyElephant[2]->init(glm::ivec2(115 * 16, 33 * 16), texProgram);
		enemyElephant[2]->setMap(map);

		enemyElephant[3] = new EnemyElephant();
		enemyElephant[3]->init(glm::ivec2(100 * 16, 64 * 16), texProgram);
		enemyElephant[3]->setMap(map);

		enemyElephant[4] = new EnemyElephant();
		enemyElephant[4]->init(glm::ivec2(75 * 16, 64 * 16), texProgram);
		enemyElephant[4]->setMap(map);

		enemyElephant[5] = new EnemyElephant();
		enemyElephant[5]->init(glm::ivec2(51 * 16, 62 * 16), texProgram);
		enemyElephant[5]->setMap(map);

		enemyElephant[6] = new EnemyElephant();
		enemyElephant[6]->init(glm::ivec2(26 * 16, 64 * 16), texProgram);
		enemyElephant[6]->setMap(map);


		//Snails
		enemySnail[0] = new EnemySnail();
		enemySnail[0]->init(glm::ivec2(94 * 16, 35 * 16), texProgram, LEFT);
		enemySnail[0]->setMap(map);

		enemySnail[1] = new EnemySnail();
		enemySnail[1]->init(glm::ivec2(104 * 16, 34 * 16), texProgram, LEFT);
		enemySnail[1]->setMap(map);

		enemySnail[2] = new EnemySnail();
		enemySnail[2]->init(glm::ivec2(118 * 16, 35 * 16), texProgram, RIGHT);
		enemySnail[2]->setMap(map);

		enemySnail[3] = new EnemySnail();
		enemySnail[3]->init(glm::ivec2(138 * 16, 49 * 16), texProgram, RIGHT);
		enemySnail[3]->setMap(map);

		enemySnail[4] = new EnemySnail();
		enemySnail[4]->init(glm::ivec2(131 * 16, 49 * 16), texProgram, UP_LEFT);
		enemySnail[4]->setMap(map);

		enemySnail[5] = new EnemySnail();
		enemySnail[5]->init(glm::ivec2(134 * 16, 59 * 16), texProgram, LEFT);
		enemySnail[5]->setMap(map);

		enemySnail[6] = new EnemySnail();
		enemySnail[6]->init(glm::ivec2(139 * 16, 56 * 16), texProgram, DOWN_RIGHT);
		enemySnail[6]->setMap(map);

		enemySnail[7] = new EnemySnail();
		enemySnail[7]->init(glm::ivec2(142 * 16, 64 * 16), texProgram, UP_RIGHT);
		enemySnail[7]->setMap(map);

		enemySnail[8] = new EnemySnail();
		enemySnail[8]->init(glm::ivec2(137 * 16, 63 * 16), texProgram, RIGHT_UPSIDE);
		enemySnail[8]->setMap(map);


		//Boss
		boss = new Boss();
		boss->init(glm::ivec2(55 * 16, 7 * 16), player, texProgram);
		boss->setMap(map);

		totem = new Totem();
		totem->init(texProgram);
		totem->setTileMap(map);

	//Init sound
	soundEngine = createIrrKlangDevice();
	if (soundEngine) {
		if (backgroundMusic) backgroundMusic->stop();
		backgroundMusic = soundEngine->play2D("sounds/08_Forest Test.wav", true, false, true);
	}
	updateScreen();
}

bool GameScene::checkIfOnScreen(glm::ivec2 pos, glm::ivec2 size) {
	int margin = SCREEN_MARGIN_UPDATE;

	int right = leftCam + 16 * 16;
	int bottom = topCam + 15 * 16;

	// Comprova si l'objecte esta fora de la pantalla
	if (pos.x + size.x < leftCam - margin || pos.x > right+margin ||
		pos.y + size.y < topCam-margin || pos.y > bottom+margin) {
		return false;  // Fuera de pantalla
	}

	return true;  // Está dins
}

//Comprova totes les entitats i mira si estan a la camara
void GameScene::updateScreen() {
	//Check if Enemies are on screen
	for (EnemyFlower* f : enemyFlower) {
		f->setOnScreen(checkIfOnScreen(f->getPosition(), f->getSize()));
	}

	for (EnemyElephant* e : enemyElephant) {
		e->setOnScreen(checkIfOnScreen(e->getPosition(), e->getSize()));
	}

	for (EnemySnail* s : enemySnail) {
		s->setOnScreen(checkIfOnScreen(s->getPosition(), s->getSize()));
	}

	boss->setOnScreen(player->isOnBossfight());

	//Check if items are on screen
	for (SmallHeart* sh : smallHeart) {
		sh->setOnScreen(checkIfOnScreen(sh->getPosition(), sh->getColliderSize()));
	}

	for (BigHeart* bh : bigHeart) {
		bh->setOnScreen(checkIfOnScreen(bh->getPosition(), bh->getColliderSize()));
	}

	for (Gourd* g : gourd) {
		g->setOnScreen(checkIfOnScreen(g->getPosition(), g->getColliderSize()));
	}

	for (Armor* a : armor) {
		a->setOnScreen(checkIfOnScreen(a->getPosition(), a->getColliderSize()));
	}

	for (Helmet* h : helmet) {
		h->setOnScreen(checkIfOnScreen(h->getPosition(), h->getColliderSize()));
	}
}

void GameScene::spawnItem(glm::ivec2 pos, glm::ivec2 sizeEnemy)
{
	int num = rand() % 100;

	if (num < ITEM_GENERATION_PROBABILITY) {
		num = rand() % NUM_ITEMS;
		cout << "Generated item " << num << endl;
		if (num == SMALL_HEART) {
			SmallHeart* item = new SmallHeart();
			item->init(pos, texProgram);
			item->setTileMap(map);
			smallHeart.push_back(item);
			//cout << "The heart was generated on position: " << item->getPosition().x << ' ' << item->getPosition().y << endl;
		}
		else if (num == BIG_HEART) {
			BigHeart* item = new BigHeart();
			item->init(pos, texProgram);
			item->setTileMap(map);
			bigHeart.push_back(item);
		}
		else if (num == ARMOR) {
			Armor* item = new Armor();
			item->init(pos, texProgram);
			item->setTileMap(map);
			armor.push_back(item);
		}
		else if (num == HELMET) {
			Helmet* item = new Helmet();
			item->init(pos, texProgram);
			item->setTileMap(map);
			helmet.push_back(item);
		}
		else if (num == GOURD) {
			Gourd* item = new Gourd();
			item->init(pos, texProgram);
			item->setTileMap(map);
			gourd.push_back(item);
		}
	}
}

bool GameScene::collidesWithPlayer(glm::ivec2 posColliderEnemy, glm::ivec2 sizeColliderEnemy) {
	//Per gestionara atac, els colliders aquests seran diferents en gunció de si el player ataca (gestio es fa a player.cpp)
	glm::ivec2 posColliderPlayer = player->getColliderPosition();
	glm::ivec2 sizeColliderPlayer = player->getColliderSize();

	//printf("POS_COLIDER_PLAYER:%d %d\n", posColliderPlayer.x, posColliderPlayer.y);
	//printf("POS_COLIDER_ENEMY:%d %d\n", posColliderEnemy.x, posColliderEnemy.y);
	//printf("SIZE_COLIDER_PLAYER:%d %d\n", sizeColliderPlayer.x, sizeColliderPlayer.y);
	//printf("SIZE_COLIDER_ENEMY:%d %d\n\n", sizeColliderEnemy.x, sizeColliderEnemy.y);


	bool overlapX = (posColliderEnemy.x < posColliderPlayer.x + sizeColliderPlayer.x) &&
		(posColliderEnemy.x + sizeColliderEnemy.x > posColliderPlayer.x);

	bool overlapY = (posColliderEnemy.y < posColliderPlayer.y + sizeColliderPlayer.y) &&
		(posColliderEnemy.y + sizeColliderEnemy.y > posColliderPlayer.y);

	return overlapX && overlapY;
}

bool GameScene::collidesWithPlayerItem(glm::ivec2 posColliderEnemy, glm::ivec2 sizeColliderEnemy)
{
	//Duplicat pero es necessita per els items (pq sino fa q els puguis agafar amb els atacs)
	glm::ivec2 posColliderPlayer = player->getColliderPositionNeutral();
	glm::ivec2 sizeColliderPlayer = player->getColliderSizeNeutral();

	//printf("POS_COLIDER_PLAYER:%d %d\n", posColliderPlayer.x, posColliderPlayer.y);
	//printf("POS_COLIDER_ENEMY:%d %d\n", posColliderEnemy.x, posColliderEnemy.y);
	//printf("SIZE_COLIDER_PLAYER:%d %d\n", sizeColliderPlayer.x, sizeColliderPlayer.y);
	//printf("SIZE_COLIDER_ENEMY:%d %d\n\n", sizeColliderEnemy.x, sizeColliderEnemy.y);


	bool overlapX = (posColliderEnemy.x < posColliderPlayer.x + sizeColliderPlayer.x) &&
		(posColliderEnemy.x + sizeColliderEnemy.x > posColliderPlayer.x);

	bool overlapY = (posColliderEnemy.y < posColliderPlayer.y + sizeColliderPlayer.y) &&
		(posColliderEnemy.y + sizeColliderEnemy.y > posColliderPlayer.y);

	return overlapX && overlapY;
}

void GameScene::updateEnemy(int deltaTime, Enemy* enemy) {
	enemy->update(deltaTime);
	//if (enemy == enemyElephant[0])cout << "I'm elephant 0" << endl;
	//static int a = 0;
	if (player->getIsAttacking2() && !enemy->isInvencible() && collidesWithPlayer(enemy->getColliderPosition(), enemy->getColliderSize())) {
		//cout << "I got Damaged on iteration " << endl;
		bool dead = enemy->getHurt(player->getDamage());
		if (dead && enemy != boss) {
			spawnItem(enemy->getPosition(), enemy->getSize());
			delete enemy;
		}
		else if (dead && enemy == boss) {
			bossScreenShake = true;
		}
	}
	else if (!godModeOn && !enemy->isInvencible() && !player->isInvencible() && collidesWithPlayer(enemy->getColliderPosition(), enemy->getColliderSize())) {
		player->getHurt(enemy->getDamage());
	}
	else if (!godModeOn && enemy->getHasBullet() && enemy != boss && !player->isInvencible() && collidesWithPlayer(enemy->getBullet()->getColliderPosition(), enemy->getBullet()->getColliderSize())) {
		player->getHurt(enemy->getDamage());
		enemy->getBullet()->deactivate();
		//cout << "HURT BY BULLET" << endl;
	}
	else if (!godModeOn && enemy->getHasBullet() && enemy == boss && !player->isInvencible()) {
		for (int i = 0; i < 4; ++i) {
			if (collidesWithPlayer(boss->getBullet(i)->getColliderPosition(), boss->getBullet(i)->getColliderSize())) {
				cout << "Hit by bullet" << endl;
				player->getHurt(enemy->getDamage());
				boss->getBullet(i)->deactivate();
			}
		}
		//cout << "HURT BY BULLET" << endl;
	}
	//++a;
}

void GameScene::updateEnemiesOnScreen(int deltaTime) {
	//Para EnemyFlowers
	for (EnemyFlower* f : enemyFlower) {
		if (f->isOnScreen()) updateEnemy(deltaTime, f);
	}

	for (EnemyElephant* e : enemyElephant) {
		if (e->isOnScreen()) updateEnemy(deltaTime, e);
	}

	for (EnemySnail* s : enemySnail) {
		if (s->isOnScreen()) updateEnemy(deltaTime, s);
	}

	if (boss->isOnScreen() && !bossDead) updateEnemy(deltaTime, boss);
}

void GameScene::updateItems(int deltaTime) {
	for (SmallHeart* sh : smallHeart) {
		if (sh->isOnScreen()) {
			sh->update(deltaTime);
			if (collidesWithPlayerItem(sh->getColliderPosition(), sh->getColliderSize())) {
				player->updateHealth(3);
				sh->active = false;
			}
		}
	}

	for (BigHeart* bh : bigHeart) {
		if (bh->isOnScreen()) {
			bh->update(deltaTime);
			if (collidesWithPlayerItem(bh->getColliderPosition(), bh->getColliderSize())) {
				player->updateHealth(-1);
				bh->active = false;
			}
		}
	}

	for (Gourd* g : gourd) {
		if (g->isOnScreen()) {
			g->update(deltaTime);
			if (collidesWithPlayerItem(g->getColliderPosition(), g->getColliderSize())) {
				player->updateGourds();
				g->active = false;
			}
		}
	}

	for (Armor* a : armor) {
		if (a->isOnScreen()) {
			a->update(deltaTime);
			if (collidesWithPlayerItem(a->getColliderPosition(), a->getColliderSize())) {
				player->setInvencible();
				a->active = false;
			}
		}
	}

	for (Helmet* h : helmet) {
		if (h->isOnScreen()) {
			h->update(deltaTime);
			if (collidesWithPlayerItem(h->getColliderPosition(), h->getColliderSize())) {
				player->setDefensiveHits(4);
				h->active = false;
			}
		}
	}

	if (totem->isOnScreen()) {
		totemUpTimer -= deltaTime;
		if (totemUpTimer < 0) {
			totem->update(deltaTime);
		}
		else {
			totem->setPosition(glm::ivec2(totem->getPosition().x, totem->getPosition().y - 1));
		}

		if (collidesWithPlayerItem(totem->getColliderPosition(), totem->getColliderSize()) && !endAnimation) {
			endAnimation = true;
			totem->active = false;
		}
	}
}

void GameScene::updateUI(int deltaTime) {
	ui->update(deltaTime, glm::vec2(leftCam, topCam), spear);
	if (player->isOnBossfight()) ui->setBossfight();
	ui->setPlayerHealth(player->getHealth());
	ui->setPlayerMaxHealth(player->getMaxHealth());
	ui->setPlayerDefensiveHits(player->getDefensiveHits());
	ui->setPlayerAttackingHits(player->getAttackingHits());
	ui->setPlayerPotions(player->getLives());
	if (player->isOnBossfight()) ui->setBossHealth(boss->getHealth());
}

void GameScene::update(int deltaTime)
{
	if (bossScreenShake) {
		timerScreenShake -= deltaTime;
		if (timerScreenShake < 0) {
			bossScreenShake = false;
			bossDying = true;
		}
		else handleScreenShake();
	}
	else if (bossDying) {
		bool end = boss->dieAnimation(deltaTime);
		if (end) {
			bossDying = false;
			bossDead = true;

			totem->setPosition(boss->getColliderPosition());
			totem->active = true;
			totem->setOnScreen(true);
		}
	}
	else if (endAnimation) {
		endAnimationTimer -= deltaTime;
		if (endAnimationTimer > 0) {
			player->endAnimation(deltaTime);
		}
		else {
			//Canviar a creditos
			cout << "CREDITOS" << endl;
		}
	}
	else if (!paused) {
		currentTime += deltaTime;
		player->update(deltaTime);
		updateEnemiesOnScreen(deltaTime);
		updateItems(deltaTime);
		updateUI(deltaTime);

		if (!player->isOnBossfight()) handleCamera();
		else if (!music) {
			if (backgroundMusic) backgroundMusic->stop();
			backgroundMusic = soundEngine->play2D("sounds/11_Boss Battle.wav", true, false, true);
			music = true;
		}
		updateScreen();

		glm::vec2 cameraPos = glm::vec2(leftCam, topCam);
		ui->update(deltaTime, cameraPos, spear);
	}
}

void GameScene::renderEnemiesOnScreen() {
	for (EnemyFlower* f : enemyFlower) {
		if (f->isOnScreen()) f->render();
	}

	for (EnemyElephant* e : enemyElephant) {
		if (e->isOnScreen()) e->render();
	}

	for (EnemySnail* s : enemySnail) {
		if (s->isOnScreen()) s->render();
	}

	if (boss->isOnScreen() && !bossDead) boss->render();
}

void GameScene::renderItems() {
	for (SmallHeart* sh : smallHeart) {
		if (sh->isOnScreen()) sh->render();
	}
	for (BigHeart* bh : bigHeart) {
		if (bh->isOnScreen()) bh->render();
	}
	for (Gourd* g : gourd) {
		if (g->isOnScreen()) g->render();
	}
	for (Armor* a : armor) {
		if(a->isOnScreen()) a->render();
	}
	for (Helmet* h : helmet) {
		if (h->isOnScreen()) h->render();
	}
	if (totem->isOnScreen()) totem->render();
}

void GameScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	if (!paused) texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	else texProgram.setUniform4f("color", 0.3f, 0.3f, 0.3f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();
	map->render();
	renderEnemiesOnScreen();
	renderItems();
	player->render();
	ui->render();

	if (paused) {
		pauseScreenSprite->setPosition(glm::ivec2(leftCam, topCam));
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		pauseScreenSprite->render();
	}
}

void GameScene::handleKeyPress(int key)
{
	if (!paused) {
		if (key == GLFW_KEY_C) {
			spear = !spear;
			if (spear) player->setWeapon(SPEAR);
			else player->setWeapon(FIRE);
		}
		if (key == GLFW_KEY_G) {
			godModeOn = !godModeOn;
			player->godModeOn = godModeOn;
		}
		if (key == GLFW_KEY_H) player->healCheat();
	}
	if (key == GLFW_KEY_P) paused = !paused;
}

void GameScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void GameScene::handleCamera()
{
	glm::ivec2 posPlayer = player->getPosition();

	if (!verticalScroll) {
		leftCam = posPlayer.x - 8 * 16;
		//Limit esquerra de Middle
		if (topCam == TOP_HORIZONTAL_MIDDLE && leftCam < 32 * 16) leftCam = 32 * 16;
		//controlen Segons la posicio del jugador si s'ha entrat en zona vertical
		else if (posPlayer.x >= LEFT_VERTICAL_RIGHT - 12) {
			verticalScroll = true;
			leftCam = LEFT_VERTICAL_RIGHT;
		}
		else if (posPlayer.x <= LEFT_VERTICAL_LEFT + 16 * 16 - 12) {
			verticalScroll = true;
			leftCam = LEFT_VERTICAL_LEFT;
		}

		else if (topCam == TOP_IN_TOP_OF_MAP && posPlayer.x > LEFT_BOSSFIGHT) {
			leftCam = LEFT_BOSSFIGHT;
			player->setOnBossfight(true);
		}

		else if (topCam == TOP_IN_TOP_OF_MAP && leftCam >= LEFT_BOSSFIGHT - 16 * 16) 
			leftCam = LEFT_BOSSFIGHT - 16 * 16;

		//Es bloqueja camara en limit zona horitzontal
		else if (leftCam >= 112 * 16) leftCam = 112 * 16;
		else if (leftCam <= 16 * 16) leftCam = 16 * 16;
	}

	else {
		topCam = posPlayer.y - 7 * 16;
		//Bloqueig camara al entrar en zona vertical
		if (leftCam == LEFT_VERTICAL_RIGHT && topCam < TOP_HORIZONTAL_MIDDLE) topCam = TOP_HORIZONTAL_MIDDLE;
		else if (topCam <= 0) topCam = 0;
		if (posPlayer.x < LEFT_VERTICAL_RIGHT - 12 && posPlayer.x > LEFT_VERTICAL_LEFT + 16 * 16 - 12) {
			verticalScroll = false;
			leftCam = posPlayer.x;
		}
		if (topCam >= TOP_IN_BOTTOM_OF_MAP) topCam = TOP_IN_BOTTOM_OF_MAP;
		else if (topCam <= TOP_IN_TOP_OF_MAP) topCam = TOP_IN_TOP_OF_MAP;
	}

	projection = glm::ortho(leftCam, leftCam + 16 * 16, topCam + 15 * 16, topCam);
}

void GameScene::handleScreenShake()
{
	if (screenShakeDown) {
		topCam++;
		if (topCam >= 6) screenShakeDown = false;
	}
	else {
		topCam--;
		if (topCam <= 0) screenShakeDown = true;
	}
	projection = glm::ortho(leftCam, leftCam + 16 * 16, topCam + 15 * 16, topCam);
}



