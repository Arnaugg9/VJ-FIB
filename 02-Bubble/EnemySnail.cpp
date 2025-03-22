#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "EnemySnail.h"
#include "Game.h"

#define MIN_TIME_CHANGE_DIR 2000
#define CHANGE_DIR_PROBABILITY 35

enum anims {
	MOVING_LEFT, MOVING_LEFT_UPSIDE, MOVING_RIGHT, MOVING_RIGHT_UPSIDE, MOVING_UP_LEFT_SIDE, MOVING_UP_RIGHT_SIDE,
	MOVING_DOWN_LEFT_SIDE, MOVING_DOWN_RIGHT_SIDE, HIDE_LEFT, HIDE_LEFT_UPSIDE, HIDE_RIGHT, HIDE_RIGHT_UPSIDE,
	HIDE_UP_LEFT_SIDE, HIDE_UP_RIGHT_SIDE, HIDE_DOWN_LEFT_SIDE, HIDE_DOWN_RIGHT_SIDE
};

enum Directions
{
	LEFT, RIGHT, UP, DOWN
};

enum Walls
{
	BOTTOM_WALL, TOP_WALL, LEFT_WALL, RIGHT_WALL
};

void EnemySnail::init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram)
{
	Enemy::init(enemyPos, shaderProgram);

	//Caracteristiques comunes
	sizeHorizontal = glm::ivec2(21, 16);
	sizeVertical = glm::ivec2(16, 21);
	sizeEnemy = sizeHorizontal;

	sizeColliderHorizontal = glm::ivec2(17, 13);
	sizeColliderVertical = glm::ivec2(13, 17);

	damage = 1;
	health = 1;

	timeChangeDirection = MIN_TIME_CHANGE_DIR;

	posCollider = posEnemy;
	posCollider.x += 2;
	posCollider.y -= 2;
	sizeCollider = sizeColliderHorizontal;

	int direction = LEFT;

	sprite->setNumberAnimations(16);

	//MOVE_LEFT
	sprite->setAnimationSpeed(MOVING_LEFT, 4);
	sprite->addKeyframe(MOVING_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVING_LEFT, glm::vec2(0.125f, 0.f));

	//MOVE_LEFT_UPSIDE
	sprite->setAnimationSpeed(MOVING_LEFT_UPSIDE, 4);
	sprite->addKeyframe(MOVING_LEFT_UPSIDE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVING_LEFT_UPSIDE, glm::vec2(0.625f, 0.f));

	//MOVE_RIGHT
	sprite->setAnimationSpeed(MOVING_RIGHT, 4);
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.0f, 0.125f));
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.125f, 0.125f));

	//MOVE_RIGHT
	sprite->setAnimationSpeed(MOVING_RIGHT_UPSIDE, 4);
	sprite->addKeyframe(MOVING_RIGHT_UPSIDE, glm::vec2(0.5f, 0.125f));
	sprite->addKeyframe(MOVING_RIGHT_UPSIDE, glm::vec2(0.625f, 0.125f));

	//MOVE_UP_LEFT_SIDE
	sprite->setAnimationSpeed(MOVING_UP_LEFT_SIDE, 4);
	sprite->addKeyframe(MOVING_UP_LEFT_SIDE, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVING_UP_LEFT_SIDE, glm::vec2(0.125f, 0.25f));

	//MOVE_UP_RIGHT_SIDE
	sprite->setAnimationSpeed(MOVING_UP_RIGHT_SIDE, 4);
	sprite->addKeyframe(MOVING_UP_RIGHT_SIDE, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(MOVING_UP_RIGHT_SIDE, glm::vec2(0.625f, 0.25f));

	//MOVE_DOWN_LEFT_SIDE
	sprite->setAnimationSpeed(MOVING_DOWN_LEFT_SIDE, 4);
	sprite->addKeyframe(MOVING_DOWN_LEFT_SIDE, glm::vec2(0.0f, 0.375f));
	sprite->addKeyframe(MOVING_DOWN_LEFT_SIDE, glm::vec2(0.125f, 0.375f));

	//MOVE_DOWN_RIGHT_SIDE
	sprite->setAnimationSpeed(MOVING_DOWN_RIGHT_SIDE, 4);
	sprite->addKeyframe(MOVING_DOWN_RIGHT_SIDE, glm::vec2(0.5f, 0.375f));
	sprite->addKeyframe(MOVING_DOWN_RIGHT_SIDE, glm::vec2(0.625f, 0.375f));


	//HIDE_LEFT
	sprite->setAnimationSpeed(HIDE_LEFT, 4);
	sprite->addKeyframe(HIDE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(HIDE_LEFT, glm::vec2(0.375f, 0.f));

	//HIDE_LEFT_UPSIDE
	sprite->setAnimationSpeed(HIDE_LEFT_UPSIDE, 4);
	sprite->addKeyframe(HIDE_LEFT_UPSIDE, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(HIDE_LEFT_UPSIDE, glm::vec2(0.875f, 0.f));

	//HIDE_RIGHT
	sprite->setAnimationSpeed(HIDE_RIGHT, 4);
	sprite->addKeyframe(HIDE_RIGHT, glm::vec2(0.25f, 0.125f));
	sprite->addKeyframe(HIDE_RIGHT, glm::vec2(0.375f, 0.125f));

	//HIDE_RIGHT_UPSIDE
	sprite->setAnimationSpeed(HIDE_RIGHT_UPSIDE, 4);
	sprite->addKeyframe(HIDE_RIGHT_UPSIDE, glm::vec2(0.75f, 0.125f));
	sprite->addKeyframe(HIDE_RIGHT_UPSIDE, glm::vec2(0.875f, 0.125f));

	//HIDE_UP_LEFT_SIDE
	sprite->setAnimationSpeed(HIDE_UP_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDE_UP_LEFT_SIDE, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(HIDE_UP_LEFT_SIDE, glm::vec2(0.375f, 0.25f));

	//HIDE_UP_RIGHT_SIDE
	sprite->setAnimationSpeed(HIDE_UP_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDE_UP_LEFT_SIDE, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(HIDE_UP_LEFT_SIDE, glm::vec2(0.875f, 0.25f));

	//HIDE_DOWN_LEFT_SIDE
	sprite->setAnimationSpeed(HIDE_DOWN_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDE_DOWN_LEFT_SIDE, glm::vec2(0.25f, 0.375f));
	sprite->addKeyframe(HIDE_DOWN_LEFT_SIDE, glm::vec2(0.375f, 0.375f));

	//HIDE_DOWN_RIGHT_SIDE
	sprite->setAnimationSpeed(HIDE_DOWN_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDE_DOWN_LEFT_SIDE, glm::vec2(0.75f, 0.375f));
	sprite->addKeyframe(HIDE_DOWN_LEFT_SIDE, glm::vec2(0.875f, 0.375f));

	sprite->changeAnimation(MOVING_LEFT);
	sprite->setPosition(posEnemy);

	//Caracteristiques especifiques Snail
	hiding = false;
	movingVertical = false;
}

void EnemySnail::update(int deltaTime)
{
	Enemy::update(deltaTime);
	timeChangeDirection -= deltaTime;
	if (!hiding) handleMovement(deltaTime);
	handleHiding(deltaTime);

	posCollider = posEnemy;
	posCollider.x += 2;
	posCollider.y -= 4;
}

void EnemySnail::render()
{
	Enemy::render();
}

void EnemySnail::handleMovement(int deltaTime)
{
	if (!movingVertical) moveH(deltaTime);
	else moveV(deltaTime);
}

void EnemySnail::moveV(int deltaTime)
{

}

void EnemySnail::moveH(int deltaTime)
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

	//No tenim terra a sota
	if (!map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y)) {
		direction = DOWN;
		sizeEnemy = sizeVertical;
		sizeCollider = sizeColliderVertical;
		movingVertical = true;
	}

	//No choquem a l'esquerra
	if (direction == LEFT && map->collisionMoveLeft(posCollider, sizeCollider)) {
		//No tenim terra a sota
		if (!map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y)) {
			direction = DOWN;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;
		}
	}
	//Choquem a l'esquerra
	else {
	
	}


	if ((direction == LEFT && map->collisionMoveLeft(posCollider, sizeCollider)) ||
		(direction == RIGHT && map->collisionMoveRight(posCollider, sizeCollider))) {

		//Si xoca canvia la posicio
		direction = (direction == LEFT) ? RIGHT : LEFT;
		timeChangeDirection = MIN_TIME_CHANGE_DIR;
		posEnemy.x += direction == LEFT ? -moveSpeed : moveSpeed;  // Corrige la posición
		posCollider.x = posEnemy.x + 2;
	}

	int new_anim;
	if(!movingVertical) new_anim = direction == LEFT ? MOVING_LEFT : MOVING_RIGHT;
	else new_anim = direction == UP ? MOVING_UP : MOVING_DOWN;

	if (new_anim != sprite->animation())
		sprite->changeAnimation(new_anim);
}

void EnemySnail::handleHiding(int deltaTime)
{
}

void EnemySnail::hide()
{
}


