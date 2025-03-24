#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "EnemySnail.h"
#include "Game.h"

#define MIN_TIME_CHANGE_DIR 2000
#define CHANGE_DIR_PROBABILITY 0

#define	GROUND_CHECK_DIST 2

enum anims {
	MOVING_LEFT, MOVING_LEFT_UPSIDE, MOVING_RIGHT, MOVING_RIGHT_UPSIDE, MOVING_UP_LEFT_SIDE, MOVING_UP_RIGHT_SIDE,
	MOVING_DOWN_LEFT_SIDE, MOVING_DOWN_RIGHT_SIDE, HIDE_LEFT, HIDE_LEFT_UPSIDE, HIDE_RIGHT, HIDE_RIGHT_UPSIDE,
	HIDE_UP_LEFT_SIDE, HIDE_UP_RIGHT_SIDE, HIDE_DOWN_LEFT_SIDE, HIDE_DOWN_RIGHT_SIDE
};

enum Directions
{
	LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, LEFT_UPSIDE, RIGHT_UPSIDE
};

void EnemySnail::init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram)
{
	Enemy::init(enemyPos, shaderProgram);

	//Caracteristiques comunes
	sizeHorizontal = glm::ivec2(21, 16);
	sizeVertical = glm::ivec2(16, 21);
	sizeEnemy = sizeHorizontal;

	posCollider = posEnemy;
	posCollider.x += 2;
	posCollider.y += 2;

	sizeColliderHorizontal = glm::ivec2(17, 14);
	sizeColliderVertical = glm::ivec2(14, 17);
	sizeCollider = sizeColliderHorizontal;

	damage = 1;
	health = 1;

	direction = LEFT;
	timeChangeDirection = MIN_TIME_CHANGE_DIR;

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
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.0f, 0.0625f));
	sprite->addKeyframe(MOVING_RIGHT, glm::vec2(0.125f, 0.0625f));

	//MOVE_RIGHT
	sprite->setAnimationSpeed(MOVING_RIGHT_UPSIDE, 4);
	sprite->addKeyframe(MOVING_RIGHT_UPSIDE, glm::vec2(0.5f, 0.0625f));
	sprite->addKeyframe(MOVING_RIGHT_UPSIDE, glm::vec2(0.625f, 0.0625f));

	//MOVE_UP_LEFT_SIDE
	sprite->setAnimationSpeed(MOVING_UP_LEFT_SIDE, 4);
	sprite->addKeyframe(MOVING_UP_LEFT_SIDE, glm::vec2(0.f, 0.125f));
	sprite->addKeyframe(MOVING_UP_LEFT_SIDE, glm::vec2(0.125f, 0.125f));

	//MOVE_UP_RIGHT_SIDE
	sprite->setAnimationSpeed(MOVING_UP_RIGHT_SIDE, 4);
	sprite->addKeyframe(MOVING_UP_RIGHT_SIDE, glm::vec2(0.5f, 0.125f));
	sprite->addKeyframe(MOVING_UP_RIGHT_SIDE, glm::vec2(0.625f, 0.125f));

	//MOVE_DOWN_LEFT_SIDE
	sprite->setAnimationSpeed(MOVING_DOWN_LEFT_SIDE, 4);
	sprite->addKeyframe(MOVING_DOWN_LEFT_SIDE, glm::vec2(0.0f, 0.1875f));
	sprite->addKeyframe(MOVING_DOWN_LEFT_SIDE, glm::vec2(0.125f, 0.1875f));

	//MOVE_DOWN_RIGHT_SIDE
	sprite->setAnimationSpeed(MOVING_DOWN_RIGHT_SIDE, 4);
	sprite->addKeyframe(MOVING_DOWN_RIGHT_SIDE, glm::vec2(0.5f, 0.1875f));
	sprite->addKeyframe(MOVING_DOWN_RIGHT_SIDE, glm::vec2(0.625f, 0.1875f));


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
	sprite->addKeyframe(HIDE_RIGHT, glm::vec2(0.25f, 0.0625f));
	sprite->addKeyframe(HIDE_RIGHT, glm::vec2(0.375f, 0.0625f));

	//HIDE_RIGHT_UPSIDE
	sprite->setAnimationSpeed(HIDE_RIGHT_UPSIDE, 4);
	sprite->addKeyframe(HIDE_RIGHT_UPSIDE, glm::vec2(0.75f, 0.0625f));
	sprite->addKeyframe(HIDE_RIGHT_UPSIDE, glm::vec2(0.875f, 0.0625f));

	//HIDE_UP_LEFT_SIDE
	sprite->setAnimationSpeed(HIDE_UP_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDE_UP_LEFT_SIDE, glm::vec2(0.25f, 0.125f));
	sprite->addKeyframe(HIDE_UP_LEFT_SIDE, glm::vec2(0.375f, 0.125f));

	//HIDE_UP_RIGHT_SIDE
	sprite->setAnimationSpeed(HIDE_UP_RIGHT_SIDE, 4);
	sprite->addKeyframe(HIDE_UP_RIGHT_SIDE, glm::vec2(0.75f, 0.125f));
	sprite->addKeyframe(HIDE_UP_RIGHT_SIDE, glm::vec2(0.875f, 0.125f));

	//HIDE_DOWN_LEFT_SIDE
	sprite->setAnimationSpeed(HIDE_DOWN_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDE_DOWN_LEFT_SIDE, glm::vec2(0.25f, 0.1875f));
	sprite->addKeyframe(HIDE_DOWN_LEFT_SIDE, glm::vec2(0.375f, 0.1875f));

	//HIDE_DOWN_RIGHT_SIDE
	sprite->setAnimationSpeed(HIDE_DOWN_RIGHT_SIDE, 4);
	sprite->addKeyframe(HIDE_DOWN_RIGHT_SIDE, glm::vec2(0.75f, 0.1875f));
	sprite->addKeyframe(HIDE_DOWN_RIGHT_SIDE, glm::vec2(0.875f, 0.1875f));

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
	posCollider.y += 2;
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
		if (num < CHANGE_DIR_PROBABILITY) {
			switch (direction)
			{
			case LEFT:
				direction = RIGHT;
				break;
			case RIGHT:
				direction = LEFT;
				break;
			case LEFT_UPSIDE:
				direction = RIGHT_UPSIDE;
				break;
			case RIGHT_UPSIDE:
				direction = LEFT_UPSIDE;
				break;

			default:
				break;
			}
		}
		timeChangeDirection = MIN_TIME_CHANGE_DIR;
	}

	int moveSpeed = 1;
	posEnemy.x += (direction == LEFT || direction == LEFT_UPSIDE) ? -moveSpeed : moveSpeed;
	posCollider.x = posEnemy.x + 1;

	//Ens estavem movent a l'esquerra
	if (direction == LEFT) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveLeft(posCollider, sizeCollider)) {
			direction = UP_LEFT;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;
		}
		//No tenim terra a sota
		posEnemy.y += 2;
		cout << "Enemy y before: " << posEnemy.y << endl;
		posCollider.y = posEnemy.y + 2;
		bool isGrounded = map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y);
		cout << "Enemy y after: " << posEnemy.y << endl;
		posCollider.y = posEnemy.y + 2;
		if (!isGrounded) {
			cout << "It's not grounded" << endl;
			direction = DOWN_RIGHT;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;

		}
	}
	cout << "Direction is: " << direction << endl;

	int new_anim = sprite->animation();
	switch (direction)
	{
	case LEFT:
		new_anim = MOVING_LEFT;
		break;
	case RIGHT:
		new_anim = MOVING_RIGHT;
		break;
	case UP_LEFT:
		new_anim = MOVING_UP_LEFT_SIDE;
		break;
	case UP_RIGHT:
		new_anim = MOVING_UP_RIGHT_SIDE;
		break;
	case DOWN_LEFT:
		new_anim = MOVING_DOWN_LEFT_SIDE;
		break;
	case DOWN_RIGHT:
		new_anim = MOVING_DOWN_RIGHT_SIDE;
		break;
	case LEFT_UPSIDE:
		new_anim = MOVING_LEFT_UPSIDE;
		break;
	case RIGHT_UPSIDE:
		new_anim = MOVING_RIGHT_UPSIDE;
		break;
	default:
		break;
	}

	cout << "Animation is: " << new_anim << endl;

	if (new_anim != sprite->animation())
		sprite->changeAnimation(new_anim);
}

void EnemySnail::handleHiding(int deltaTime)
{
}

void EnemySnail::hide()
{
}


