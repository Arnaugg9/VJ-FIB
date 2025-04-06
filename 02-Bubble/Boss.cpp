#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Boss.h"
#include "Game.h"

#define TIME_TO_RISE 3000
#define TIME_TO_FALL 3500

#define TOP_IN_TOP_OF_MAP 0
#define LEFT_BOSSFIGHT 48*16

#define FALL_SPEED 3
#define RISE_SPEED 2

#define HEALTH_CHANGE_PHASE_1 18
#define PHASE_CHANGE_TIMER 3500
#define HEALTH_CHANGE_PHASE_2 8 

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

void Boss::init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram)
{
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

	damage = 4;
	health = 20;

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
		sprite->addKeyframe(IDLE_MOVE_2, glm::vec2(0.75f, 0.5f));

		//ATTACK_2
		sprite->setAnimationSpeed(ATTACK_2, 5);
		sprite->addKeyframe(ATTACK_2, glm::vec2(0.875f, 0.125f));

		//DIE
		sprite->setAnimationSpeed(DIE, 5);
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
		bulletsSprites[i]->setAnimationSpeed(NORMAL_ATTACK, 6);
		bulletsSprites[i]->setAnimationSpeed(BLUE_FIRE, 6);

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
		bulletsSprites[i]->addKeyframe(NORMAL_ATTACK, glm::vec2(0.125f, 0.25f));

		bulletsSprites[i]->changeAnimation(NORMAL_ATTACK);
	}

	for (int i = 0; i < 4; ++i) {
		bullets[i] = new Bullet();
		bullets[i]->initBoss(glm::ivec2(11, 10), i, bulletsSprites[i], 2, 500);
	}

	//Init de les carasteristiqeus propies
	phaseNumber = 0;

	//Init caracteristiques Fase 0
	timeToRise = TIME_TO_RISE/2;
	timeToFall = 0;
	rising = false;
	falling = true;

	changingPhase = false;
	phaseChangeTimer = PHASE_CHANGE_TIMER;
}

void Boss::update(int deltaTime)
{
	Enemy::update(deltaTime);

	if (phaseNumber == 0) {
		changingPhase = health <= HEALTH_CHANGE_PHASE_1;
		if (!changingPhase) phase0Control(deltaTime);
		else changePhase(deltaTime);
	}
	else if (phaseNumber == 1) {
		changingPhase = health <= HEALTH_CHANGE_PHASE_2;
		if (!changingPhase) phase1Control(deltaTime);
	}
	else if (phaseNumber == 2) {

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
		glm::ivec2 newPos = glm::ivec2(LEFT_BOSSFIGHT, TOP_IN_TOP_OF_MAP - sizeEnemy.y);
		newPos.x += rand() % (13 * 16);
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
			sprite->changeAnimation(IDLE_MOVE);
		}
		else {
			if (sprite->animation() != CHANGE_PHASE) sprite->changeAnimation(CHANGE_PHASE);
		}
	}
}

void Boss::phase1Control(int deltaTime)
{
	if (inDestination) {
		timerAttackDuration -= deltaTime;
		attack();
	}
	else 
}

void Boss::render()
{
	Enemy::render();

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
	}
	return false;
}
