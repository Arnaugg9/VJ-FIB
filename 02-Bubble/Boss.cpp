#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Boss.h"
#include "Game.h"
#include "SoundManager.h"

#define TIME_TO_RISE 3000
#define TIME_TO_FALL 3500

#define TOP_IN_TOP_OF_MAP 0
#define LEFT_BOSSFIGHT 48*16

#define FALL_SPEED 4
#define RISE_SPEED 2

#define HEALTH_CHANGE_PHASE_1 8
#define PHASE_CHANGE_TIMER 3500
#define HEALTH_CHANGE_PHASE_2 6 
#define HEALTH_RECOVER 18
#define HEALTH_RECOVER_TIMER 250

#define ATTACK_DURATION 1500

enum anims {
	FIRST_PHASE, CHANGE_PHASE, IDLE_MOVE, ATTACK, IDLE_MOVE_2, ATTACK_2, DIE
};

enum animsBullets {
	NORMAL_ATTACK, BLUE_FIRE
};

enum dirBullets {
	LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN
};

void Boss::init(const glm::ivec2& enemyPos, Player* player, ShaderProgram& shaderProgram)
{
	this->player = player;

	posEnemy = enemyPos;
	
	onScreen = false;
	hasBullet = true;

	invencible = false;

	spritesheet.loadFromFile("images/spirtesheet_boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);

	//Caracteristiques comunes
	sizeEnemy = glm::ivec2(30, 48);
	sizePhase1 = glm::ivec2(32, 44);

	damage = 3;
	health = 33;

	posCollider = posEnemy;
	posCollider.x += 4;
	posCollider.y += 3;
	sizeCollider = glm::ivec2(22, 44);
	sizeColliderPhase1 = glm::ivec2(26, 41);


	//timeChangeDirection = MIN_TIME_CHANGE_DIR;
	//int direction = LEFT;

	sprite->setNumberAnimations(7);

		//FIRST_PHASE
		sprite->setAnimationSpeed(FIRST_PHASE, 4);
		sprite->addKeyframe(FIRST_PHASE, glm::vec2(0.f, 0.125f));

		//CHANGE_PHASE
		sprite->setAnimationSpeed(CHANGE_PHASE, 6);
		sprite->addKeyframe(CHANGE_PHASE, glm::vec2(0.125f, 0.125f));
		sprite->addKeyframe(CHANGE_PHASE, glm::vec2(0.25f, 0.125f));

		//IDLE_AND_MOVE
		sprite->setAnimationSpeed(IDLE_MOVE, 5);
		sprite->addKeyframe(IDLE_MOVE, glm::vec2(0.375f, 0.125f));
		sprite->addKeyframe(IDLE_MOVE, glm::vec2(0.875f, 0.5f));

		//ATTACK
		sprite->setAnimationSpeed(ATTACK, 5);
		sprite->addKeyframe(ATTACK, glm::vec2(0.5f, 0.125f));

		//IDLE_AND_MOVE_2
		sprite->setAnimationSpeed(IDLE_MOVE_2, 5);
		sprite->addKeyframe(IDLE_MOVE_2, glm::vec2(0.625f, 0.125f));
		sprite->addKeyframe(IDLE_MOVE_2, glm::vec2(0.75f, 0.125f));

		//ATTACK_2
		sprite->setAnimationSpeed(ATTACK_2, 5);
		sprite->addKeyframe(ATTACK_2, glm::vec2(0.875f, 0.125f));

		//DIE
		sprite->setAnimationSpeed(DIE, 11);
		sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.5f));
		sprite->addKeyframe(DIE, glm::vec2(0.625f, 0.5f));
		sprite->addKeyframe(DIE, glm::vec2(0.75f, 0.5f));

	sprite->changeAnimation(FIRST_PHASE);
	sprite->setPosition(posEnemy);

	//Sprites per la corda de la fase 0
	for (int i = 0; i < 10; ++i) {
		stringSprites[i] = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
		stringSprites[i]->setNumberAnimations(1);

		stringSprites[i]->setAnimationSpeed(0, 1);
		stringSprites[i]->addKeyframe(0, glm::vec2(0.f, 0.25f));

		stringSprites[i]->changeAnimation(0);
		stringSprites[i]->setPosition(glm::ivec2(posEnemy.x + 14, posEnemy.y - 16 * (i+1)));
	}

	for (int i = 0; i < 4; ++i) {
		bulletsSprites[i] = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
		bulletsSprites[i]->setNumberAnimations(2);
		bulletsSprites[i]->setAnimationSpeed(NORMAL_ATTACK, 18);
		bulletsSprites[i]->setAnimationSpeed(BLUE_FIRE, 8);

		if (i == LEFT_UP) {
			bulletsSprites[i]->addKeyframe(NORMAL_ATTACK, glm::vec2(0.625f, 0.25f));

			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.375f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.375f, 0.375f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.375f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.875f, 0.375f));
		}
		else if (i == LEFT_DOWN) {
			bulletsSprites[i]->addKeyframe(NORMAL_ATTACK, glm::vec2(0.5f, 0.25f));

			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.25f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.25f, 0.375f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.25f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.75f, 0.375f));
		}
		else if (i == RIGHT_UP) {
			bulletsSprites[i]->addKeyframe(NORMAL_ATTACK, glm::vec2(0.25f, 0.25f));

			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.f, 0.375f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.5f, 0.375f));
		}
		else if (i == RIGHT_DOWN) {
			bulletsSprites[i]->addKeyframe(NORMAL_ATTACK, glm::vec2(0.375f, 0.25f));

			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.125f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.125f, 0.375f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.125f, 0.5f));
			bulletsSprites[i]->addKeyframe(BLUE_FIRE, glm::vec2(0.625f, 0.375f));
		}
		//bulletsSprites[i]->addKeyframe(NORMAL_ATTACK, glm::vec2(0.125f, 0.25f));

		bulletsSprites[i]->changeAnimation(NORMAL_ATTACK);
	}

	for (int i = 0; i < 4; ++i) {
		bullets[i] = new Bullet();
		bullets[i]->initBoss(glm::ivec2(11, 10), i, bulletsSprites[i], 2, 700);
	}

	glm::ivec2 centerBoss = (posEnemy) + sizeEnemy/ 2;
	posInitBullets[LEFT_UP] = glm::ivec2(centerBoss.x - 11, centerBoss.y - 10);
	posInitBullets[LEFT_DOWN] = glm::ivec2(centerBoss.x - 11, centerBoss.y);
	posInitBullets[RIGHT_UP] = glm::ivec2(centerBoss.x, centerBoss.y - 10);
	posInitBullets[RIGHT_DOWN] = glm::ivec2(centerBoss.x, centerBoss.y);

	//Init de les carasteristiqeus propies
	phaseNumber = 0;

	//Init caracteristiques Fase 0
	timeToRise = TIME_TO_RISE/2;
	timeToFall = 0;
	rising = false;
	falling = true;

	//Init caracteristiques canvi fase
	changingPhase = false;
	phaseChangeTimer = PHASE_CHANGE_TIMER;
	healthRecoverTimer = HEALTH_RECOVER_TIMER;

	//Init caracteristiques Fase 1
	inDestination = false;
	timerAttackDuration = ATTACK_DURATION;
	destination = glm::ivec2(LEFT_BOSSFIGHT + 7 * 16, 0 + 6 * 16);
	attacked = false;

	//Init caracteritiques animacio mort
	dieAnimationTimer = 250;
	startedShootDieAnimation = false;
	dieAnimationTimer2 = 800;
}

void Boss::update(int deltaTime)
{
	Enemy::update(deltaTime);

	if (phaseNumber == 0) {
		if (!changingPhase) changingPhase = health <= HEALTH_CHANGE_PHASE_1;
		if (!changingPhase) phase0Control(deltaTime);
		else changePhase(deltaTime);
	}
	else if (phaseNumber == 1) {
		changingPhase = health <= HEALTH_CHANGE_PHASE_2;
		if (!changingPhase) phase1Control(deltaTime);
		else phaseNumber = 2;
	}
	else if (phaseNumber == 2) {
		changingPhase = health <= 0;
		if (!changingPhase) phase1Control(deltaTime);
	}

}

void Boss::phase0Control(int deltaTime)
{
	if (timeToRise < 0) {
		falling = false;
		rising = true;
		timeToFall = TIME_TO_FALL;
		timeToRise = TIME_TO_RISE;
	}

	else if (timeToFall < 0) {
		rising = false;
		falling = true;
		timeToRise = TIME_TO_RISE;
		timeToFall = TIME_TO_FALL;
		glm::ivec2 playerPos = player->getColliderPositionNeutral();
		glm::ivec2 newPos = glm::ivec2(playerPos.x, TOP_IN_TOP_OF_MAP - sizeEnemy.y);
		int posX = rand() % (2 * 16);
		int dir = rand() % 2;
		if (dir == 0) newPos.x = playerPos.x - posX;
		else newPos.x = playerPos.x + posX;
		posEnemy = newPos;
	}

	if (rising) {
		timeToFall -= deltaTime;
		posEnemy.y -= RISE_SPEED;
	}
	else if (falling) {
		timeToRise -= deltaTime;
		posEnemy.y += FALL_SPEED;
		posCollider = posEnemy;
		posCollider.x += 4;
		posCollider.y += 3;
		map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y);
	}

	posCollider = posEnemy;
	posCollider.x += 4;
	posCollider.y += 3;

	sprite->setPosition(posEnemy);
	for(int i = 0; i < 10; ++i) stringSprites[i]->setPosition(glm::ivec2(posEnemy.x + 14, posEnemy.y - 16 * (i+1)));
}

void Boss::changePhase(int deltaTime)
{
	invencible = false;
	if (phaseNumber == 0) {
		phaseChangeTimer -= deltaTime;
		if (phaseChangeTimer < 0) {
			changingPhase = false;
			phaseChangeTimer = PHASE_CHANGE_TIMER;
			phaseNumber++;
			sizeEnemy = sizePhase1;
			sizeCollider = sizeColliderPhase1;
			health = HEALTH_RECOVER;
			sprite->changeAnimation(IDLE_MOVE);
			SoundManager::playSFX("sounds/effects/boss_change_phase.wav");
		}
		else {
			if (sprite->animation() != CHANGE_PHASE) sprite->changeAnimation(CHANGE_PHASE);
			if (health < HEALTH_RECOVER) {
				healthRecoverTimer -= deltaTime;
				if (healthRecoverTimer < 0) {
					health += 1;
					healthRecoverTimer = HEALTH_RECOVER_TIMER;
				}
			}
		}
	}
}

void Boss::phase1Control(int deltaTime)
{
	if (inDestination) {
		timerAttackDuration -= deltaTime;
		handleAttack(deltaTime);
	}
	else {
		moveTo(deltaTime, destination); 
	}
} 

void Boss::moveTo(int deltaTime, glm::ivec2 dest)
{
	if (phaseNumber == 1 && sprite->animation() != IDLE_MOVE) sprite->changeAnimation(IDLE_MOVE);
	if (phaseNumber == 2 && sprite->animation() != IDLE_MOVE_2) sprite->changeAnimation(IDLE_MOVE_2);

	if (posEnemy == dest) {
		inDestination = true;
		destination = glm::ivec2(LEFT_BOSSFIGHT, 0);
		destination.x += rand() % (13 * 16);
		destination.y += rand() % (13 * 16);
	}

	glm::ivec2 move(0);

	if (posEnemy.x < destination.x) move.x = 2;
	else if (posEnemy.x > destination.x) move.x = -2;

	if (posEnemy.y < destination.y) move.y = 2;
	else if (posEnemy.y > destination.y) move.y = -2;

	if (abs(destination.x - posEnemy.x) < abs(move.x))
		move.x = destination.x - posEnemy.x;

	if (abs(destination.y - posEnemy.y) < abs(move.y))
		move.y = destination.y - posEnemy.y;


	posEnemy += move;
	posCollider = posEnemy;
	posCollider.x += 4;
	posCollider.y += 3;
}

void Boss::handleAttack(int deltaTime)
{
	if (!attacked) {
		attacked = true;
		if (phaseNumber == 1) sprite->changeAnimation(ATTACK);
		else if (phaseNumber == 2) sprite->changeAnimation(ATTACK_2);

		glm::ivec2 centerBoss = posEnemy + sizeEnemy/2;
		posInitBullets[LEFT_UP] = glm::ivec2(centerBoss.x - 11, centerBoss.y - 10);
		posInitBullets[LEFT_DOWN] = glm::ivec2(centerBoss.x - 11, centerBoss.y);
		posInitBullets[RIGHT_UP] = glm::ivec2(centerBoss.x, centerBoss.y - 10);
		posInitBullets[RIGHT_DOWN] = glm::ivec2(centerBoss.x, centerBoss.y);
		for (int i = 0; i < 4; ++i) {
			bullets[i]->activateBoss(posInitBullets[i], i, NORMAL_ATTACK);
		}
	}
	if (timerAttackDuration < 0) {
		attacked = false;
		inDestination = false;
		timerAttackDuration = ATTACK_DURATION;
	}
	else {
		for (int i = 0; i < 4; ++i) {
			bullets[i]->update(deltaTime);
		}
	}
}

bool Boss::dieAnimation(int deltaTime)
{
	if (!startedShootDieAnimation) {
		if (sprite->animation() != DIE) {
			sprite->setPosition(posEnemy + 4);
			sprite->changeAnimation(DIE);
		}
		dieAnimationTimer -= deltaTime;
		if (dieAnimationTimer < 0) {
			startedShootDieAnimation = true;

			glm::ivec2 centerBoss = posEnemy + sizeEnemy / 2;
			posInitBullets[LEFT_UP] = glm::ivec2(centerBoss.x - 11, centerBoss.y - 10);
			posInitBullets[LEFT_DOWN] = glm::ivec2(centerBoss.x - 11, centerBoss.y);
			posInitBullets[RIGHT_UP] = glm::ivec2(centerBoss.x, centerBoss.y - 10);
			posInitBullets[RIGHT_DOWN] = glm::ivec2(centerBoss.x, centerBoss.y);
			for (int i = 0; i < 4; ++i) {
				bullets[i]->activateBoss(posInitBullets[i], i, BLUE_FIRE);
			}
		}
		else {
			sprite->setPosition(posEnemy + 4);
			sprite->update(deltaTime);
		}

	}
	else {
		dieAnimationTimer2 -= deltaTime;
		if (dieAnimationTimer2 < 0) {
			return true;
		}

		for (int i = 0; i < 4; ++i) {
			bullets[i]->update(deltaTime);
		}
	}
	return false;
}

void Boss::render()
{
	if (health > 0) Enemy::render();
	else if (!startedShootDieAnimation) sprite->render();

	if (phaseNumber == 0 && !changingPhase) {
		for (Sprite* s : stringSprites) s->render();
	}

	for (Bullet* b : bullets) {
		if (b->isActive()) b->render();
	}
}

Bullet* Boss::getBullet(int numBullet)
{
	return bullets[numBullet];
}

bool Boss::getHurt(int damage)
{
	if (!changingPhase) {
		health -= damage;
		invencible = true;
		if (health <= 0) return true;
		SoundManager::playSFX("sounds/effects/boss_hurt.wav");
	}
	//cout << "My health is: " << health << endl;
	return false;
}
