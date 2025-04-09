#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "EnemyElephant.h"
#include "Game.h"
#include "SoundManager.h"

#define MIN_TIME_CHANGE_DIR 2000

#define JUMP_ANGLE_STEP 4
#define LOW_JUMP_HEIGHT 40
#define HIGH_JUMP_HEIGHT 90
#define FALL_STEP 4

#define CHANGE_DIR_PROBABILITY 35
#define JUMP_PROBABILITY 40
#define HIGH_JUMP_PROBABILITY 20

#define TIME_BULLET_ALIVE 1500

#define TIME_BETWEEN_SHOOT 2250

enum anims {
	MOVING_LEFT, MOVING_RIGHT
};

enum Directions
{
	LEFT, RIGHT
};

void EnemyElephant::init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram)
{
	Enemy::init(enemyPos, shaderProgram);

	//Caracteristiques comunes
	sizeEnemy = glm::ivec2(25.f, 26.f);

	damage = 2;
	health = 10;

	hasBullet = true;

	posCollider = posEnemy;
	posCollider.x += 2;
	posCollider.y -= 4;
	sizeCollider = glm::ivec2(20, 19);

	timeChangeDirection = MIN_TIME_CHANGE_DIR;
	int direction = LEFT;

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVING_LEFT, 4);
	sprite->addKeyframe(MOVING_LEFT, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(MOVING_LEFT, glm::vec2(0.125f, 0.25f));

	sprite->setAnimationSpeed(MOVING_RIGHT, 4);
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.375f, 0.25f));

	sprite->changeAnimation(0);
	sprite->setPosition(posEnemy);

	//Caracteristiques especifiques Elefantinho
	bJumping = false;

	bulletSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.0625), &spritesheet, &shaderProgram);

	bulletSprite->setNumberAnimations(2);
	bulletSprite->setAnimationSpeed(MOVING_LEFT, 8);
	bulletSprite->addKeyframe(MOVING_LEFT, glm::vec2(0.5f, 0.25f));
	bulletSprite->setAnimationSpeed(MOVING_RIGHT, 8);
	bulletSprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.625f, 0.25f));


	bullet = new Bullet();
	bullet->init(glm::ivec2(8,8), direction, bulletSprite, 2, TIME_BULLET_ALIVE);

	timeToShoot = TIME_BETWEEN_SHOOT;
}

void EnemyElephant::update(int deltaTime)
{
	Enemy::update(deltaTime);
	timeChangeDirection -= deltaTime;
	move();
	jump();
	posCollider = posEnemy;
	posCollider.x += 2;
	posCollider.y -= 4;
	handleShoot(deltaTime);
}

void EnemyElephant::render()
{
	Enemy::render();

	if (bullet->isActive()) bullet->render();
}

Bullet* EnemyElephant::getBullet()
{
	return bullet;
}

void EnemyElephant::move()
{
	//Si ha passat molt temps potser canvia de direccio
	if (timeChangeDirection <= 0) {
		int num = rand() % 100;
		//cout << "END_TIME" << endl;
		if (num < CHANGE_DIR_PROBABILITY) direction = (direction == LEFT) ? RIGHT : LEFT;
		timeChangeDirection = MIN_TIME_CHANGE_DIR;
	}

	int moveSpeed = 1; 
	posEnemy.x += direction == LEFT ? -moveSpeed : moveSpeed;
	posCollider.x = posEnemy.x + 2;

	//cout << "POSITION: " << posEnemy.x << ' ' << posEnemy.y << endl;

	if ((direction == LEFT && map->collisionMoveLeft(posCollider, sizeCollider)) ||
		(direction == RIGHT && map->collisionMoveRight(posCollider, sizeCollider))) {

		//Si xoca canvia la posicio
		direction = (direction == LEFT) ? RIGHT : LEFT;
		timeChangeDirection = MIN_TIME_CHANGE_DIR;
		posEnemy.x += direction == LEFT ? -moveSpeed : moveSpeed;  // Corrige la posición
		posCollider.x = posEnemy.x + 2;
	}

	int new_anim = direction == LEFT ? MOVING_LEFT : MOVING_RIGHT;

	if (new_anim != sprite->animation())
		sprite->changeAnimation(new_anim);
}

void EnemyElephant::jump()
{
	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180) {
			bJumping = false;
			posEnemy.y = startY;
		}
		else {
			// Usa el tipo de salto que se determinó al iniciar el salto
			posEnemy.y = int(startY - jumpHeight * sin(3.14159f * jumpAngle / 180.f));

			if (jumpAngle > 90) {
				posCollider.y = posEnemy.y;
				bJumping = !map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y);
				posCollider.y = posEnemy.y;
			}
		}
	}
	else {
		posEnemy.y += FALL_STEP;
		posCollider.y = posEnemy.y;
		map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y);
		posCollider.y = posEnemy.y;
	}

	int num = rand() % 100;
	if (num < JUMP_PROBABILITY && !bJumping) {
		bJumping = true;
		jumpAngle = 0;
		startY = posEnemy.y;

		// Determina una sola vez si será un HIGH_JUMP o un LOW_JUMP
		jumpHeight = (rand() % 100 < HIGH_JUMP_PROBABILITY) ? HIGH_JUMP_HEIGHT : LOW_JUMP_HEIGHT;
	}
}

void EnemyElephant::shoot() {
	bullet->activate(posEnemy, direction);
	timeToShoot = TIME_BETWEEN_SHOOT;
	SoundManager::playSFX("sounds/effects/bullet.wav");
}

void EnemyElephant::handleShoot(int deltaTime)
{
	if (bullet->isActive()) {
		bullet->update(deltaTime);
		//cout << "Bullet active on: " <<  bullet->getPosition().x << ' ' << bullet->getPosition().y << endl;
	}
	if (timeToShoot <= 0) {
		shoot();
	}
	else {
		timeToShoot -= deltaTime;
	}
}
