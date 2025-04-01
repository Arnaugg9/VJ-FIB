#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "EnemySnail.h"
#include "Game.h"

#define MIN_TIME_CHANGE_DIR 2000
#define CHANGE_DIR_PROBABILITY 35

#define MIN_TIME_HIDE 3000
#define MIN_DURATION_HIDE 2000
#define END_HIDE_PROBABILITY 60
#define HIDE_PROBABILITY 30
#define HIDE_ANIMATION_DURATION 200

#define	GROUND_CHECK_DIST 2

#define AUX_SLOW_SPEED 20

enum anims {
	MOVING_LEFT, MOVING_LEFT_UPSIDE, MOVING_RIGHT, MOVING_RIGHT_UPSIDE, MOVING_UP_LEFT_SIDE, MOVING_UP_RIGHT_SIDE,
	MOVING_DOWN_LEFT_SIDE, MOVING_DOWN_RIGHT_SIDE, HIDE_LEFT, HIDE_LEFT_UPSIDE, HIDE_RIGHT, HIDE_RIGHT_UPSIDE,
	HIDE_UP_LEFT_SIDE, HIDE_UP_RIGHT_SIDE, HIDE_DOWN_LEFT_SIDE, HIDE_DOWN_RIGHT_SIDE, HIDEN_LEFT, HIDEN_LEFT_UPSIDE,
	HIDEN_RIGHT, HIDEN_RIGHT_UPSIDE, HIDEN_UP_LEFT_SIDE, HIDEN_UP_RIGHT_SIDE, HIDEN_DOWN_LEFT_SIDE, HIDEN_DOWN_RIGHT_SIDE
};

enum Directions
{
	LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, LEFT_UPSIDE, RIGHT_UPSIDE
};

void EnemySnail::init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram, int dir)
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

	damage = 2;
	health = 1;

	//Si alguna direcció es d'up ha de donar true 
	direction = dir;
	movingVertical = (direction == UP_LEFT || direction == UP_RIGHT || direction == DOWN_LEFT || direction == DOWN_RIGHT);


	timeChangeDirection = MIN_TIME_CHANGE_DIR;

	sprite->setNumberAnimations(24);

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



	//HIDEN_LEFT
	sprite->setAnimationSpeed(HIDEN_LEFT, 4);
	sprite->addKeyframe(HIDEN_LEFT, glm::vec2(0.375f, 0.f));

	//HIDEN_LEFT_UPSIDE
	sprite->setAnimationSpeed(HIDEN_LEFT_UPSIDE, 4);
	sprite->addKeyframe(HIDEN_LEFT_UPSIDE, glm::vec2(0.875f, 0.f));

	//HIDEN_RIGHT
	sprite->setAnimationSpeed(HIDEN_RIGHT, 4);
	sprite->addKeyframe(HIDEN_RIGHT, glm::vec2(0.375f, 0.0625f));

	//HIDEN_RIGHT_UPSIDE
	sprite->setAnimationSpeed(HIDEN_RIGHT_UPSIDE, 4);
	sprite->addKeyframe(HIDEN_RIGHT_UPSIDE, glm::vec2(0.875f, 0.0625f));

	//HIDEN_UP_LEFT_SIDE
	sprite->setAnimationSpeed(HIDEN_UP_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDEN_UP_LEFT_SIDE, glm::vec2(0.375f, 0.125f));

	//HIDEN_UP_RIGHT_SIDE
	sprite->setAnimationSpeed(HIDEN_UP_RIGHT_SIDE, 4);
	sprite->addKeyframe(HIDEN_UP_RIGHT_SIDE, glm::vec2(0.875f, 0.125f));

	//HIDEN_DOWN_LEFT_SIDE
	sprite->setAnimationSpeed(HIDEN_DOWN_LEFT_SIDE, 4);
	sprite->addKeyframe(HIDEN_DOWN_LEFT_SIDE, glm::vec2(0.375f, 0.1875f));

	//HIDEN_DOWN_RIGHT_SIDE
	sprite->setAnimationSpeed(HIDEN_DOWN_RIGHT_SIDE, 4);
	sprite->addKeyframe(HIDEN_DOWN_RIGHT_SIDE, glm::vec2(0.875f, 0.1875f));


	sprite->changeAnimation(MOVING_LEFT);
	sprite->setPosition(posEnemy);

	//Caracteristiques especifiques Snail
	hiding = false;
	auxSlowSpeed = AUX_SLOW_SPEED;
	//movingVertical = false;			FET a dalt
}

void EnemySnail::update(int deltaTime)
{
	Enemy::update(deltaTime);
	timeChangeDirection -= deltaTime;
	auxSlowSpeed -= deltaTime;

	if (!hiding && auxSlowSpeed <= 0) {
		handleMovement(deltaTime);
		auxSlowSpeed = AUX_SLOW_SPEED;
	}
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
	//Si ha passat molt temps potser canvia de direccio
	if (timeChangeDirection <= 0) {
		int num = rand() % 100;
		//cout << "END_TIME" << endl;
		if (num < CHANGE_DIR_PROBABILITY) {
			switch (direction)
			{
			case UP_LEFT:
				direction = DOWN_LEFT;
				break;
			case UP_RIGHT:
				direction = DOWN_RIGHT;
				break;
			case DOWN_LEFT:
				direction = UP_LEFT;
				break;
			case DOWN_RIGHT:
				direction = UP_RIGHT;
				break;

			default:
				break;
			}
		}
		timeChangeDirection = MIN_TIME_CHANGE_DIR;
	}

	int moveSpeed = 1;
	posEnemy.y += (direction == UP_LEFT || direction == UP_RIGHT) ? -moveSpeed : moveSpeed;
	posCollider.y = posEnemy.y + 2;
	int aux = 1;
	if (direction == UP_LEFT) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveUp(posCollider, sizeCollider, &posEnemy.y)) {
			direction = RIGHT_UPSIDE;
			sizeEnemy = sizeHorizontal;
			sizeCollider = sizeColliderHorizontal;
			movingVertical = false;
		}

		else {
			posEnemy.x -= 2;
			posCollider.x = posEnemy.x + 2;
			//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
			bool isGrounded = map->collisionMoveLeft(posCollider, sizeCollider);
			//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;

			if (!isGrounded) {
				//cout << "It's not grounded" << endl;
				posEnemy.x -= 2;
				posEnemy.y += sizeColliderVertical.y - sizeColliderHorizontal.y;
				direction = LEFT;
				sizeEnemy = sizeHorizontal;
				sizeCollider = sizeColliderHorizontal;
				movingVertical = false;
			}
			else {
				posEnemy.x += 2;
			}
			posCollider.x = posEnemy.x + 2;
			posCollider.y = posEnemy.y + 2;
		}
	}

	else if (direction == UP_RIGHT) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveUp(posCollider, glm::ivec2(sizeCollider.x - 2, sizeCollider.y), &posEnemy.y)) {
			direction = LEFT_UPSIDE;
			sizeEnemy = sizeHorizontal;
			sizeCollider = sizeColliderHorizontal;
			movingVertical = false;
		}
		else {
			posEnemy.x += 2;
			posCollider.x = posEnemy.x + 2;
			//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
			bool isGrounded = map->collisionMoveRight(posCollider, sizeCollider);
			//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;

			if (!isGrounded) {
				//cout << "It's not grounded" << endl;
				posEnemy.y += sizeColliderVertical.y - sizeColliderHorizontal.y;
				direction = RIGHT;
				sizeEnemy = sizeHorizontal;
				sizeCollider = sizeColliderHorizontal;
				movingVertical = false;
			}
			else {
				posEnemy.x -= 2;
			}
			posCollider.x = posEnemy.x + 2;
			posCollider.y = posEnemy.y + 2;
		}
	}

	else if (direction == DOWN_LEFT) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y)) {
			direction = RIGHT;
			posEnemy.y += sizeColliderVertical.y - sizeColliderHorizontal.y;
			sizeEnemy = sizeHorizontal;
			sizeCollider = sizeColliderHorizontal;
			movingVertical = false;
		}
		else {
			posEnemy.x -= 2;
			posCollider.x = posEnemy.x + 2;
			//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
			bool isGrounded = map->collisionMoveLeft(posCollider, sizeCollider);
			//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;

			if (!isGrounded) {
				//cout << "It's not grounded" << endl;
				posEnemy.x -= 2;
				direction = LEFT_UPSIDE;
				sizeEnemy = sizeHorizontal;
				sizeCollider = sizeColliderHorizontal;
				movingVertical = false;
			}
			else {
				posEnemy.x += 2;
			}
			posCollider.x = posEnemy.x + 2;
			posCollider.y = posEnemy.y + 2;
		}
	}

	else if (direction == DOWN_RIGHT) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y)) {
			direction = LEFT;
			posEnemy.y += sizeColliderVertical.y - sizeColliderHorizontal.y;
			sizeEnemy = sizeHorizontal;
			sizeCollider = sizeColliderHorizontal;
			movingVertical = false;
		}
		else {
			posEnemy.x += 2;
			posCollider.x = posEnemy.x + 2;
			//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
			bool isGrounded = map->collisionMoveRight(posCollider, sizeCollider);
			//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;

			if (!isGrounded) {
				//cout << "It's not grounded" << endl;
				posEnemy.x += 2;
				direction = RIGHT_UPSIDE;
				sizeEnemy = sizeHorizontal;
				sizeCollider = sizeColliderHorizontal;
				movingVertical = false;
			}
			else {
				posEnemy.x -= 2;
			}
			posCollider.x = posEnemy.x + 2;
			posCollider.y = posEnemy.y + 2;
		}
	}

	//cout << "Direction is: " << direction << endl;
	selectAnim();
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
	posCollider.x = posEnemy.x + 2;

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
		posCollider.y = posEnemy.y + 2;
		//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
		bool isGrounded = map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y);
		//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;
		posCollider.y = posEnemy.y + 2;
		if (!isGrounded) {
			posEnemy.x -= 2;
			//cout << "It's not grounded" << endl;
			posEnemy.x += sizeHorizontal.x - sizeVertical.x;
			direction = DOWN_RIGHT;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;

		}
	}
	//Ens estem movent a la dreta
	else if (direction == RIGHT) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveRight(posCollider, sizeCollider)) {
			cout << "The x should be: " << 54 * 16 - sizeColliderHorizontal.x << endl;
			cout << "But it's: " << posCollider.x << endl;
			direction = UP_RIGHT;
			posEnemy.x -= moveSpeed;
			posEnemy.x += sizeHorizontal.x - sizeVertical.x;
			posCollider.x = posEnemy.x + 2;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;
			cout << "The x should be: " << 54 * 16 - sizeColliderVertical.x << endl;
			cout << "But it's: " << posCollider.x << endl;
		}
		//No tenim terra a sota
		else {
			posEnemy.y += 2;
			posCollider.y = posEnemy.y + 2;
			//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
			bool isGrounded = map->collisionMoveDown(posCollider, sizeCollider, &posEnemy.y);
			//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;
			posCollider.y = posEnemy.y + 2;
			if (!isGrounded) {
				cout << "It's not grounded" << endl;
				direction = DOWN_LEFT;
				sizeEnemy = sizeVertical;
				sizeCollider = sizeColliderVertical;
				movingVertical = true;

			}
		}
	
	}
	//Ens estem movent a la esquerra cap per avall
	else if (direction == LEFT_UPSIDE) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveLeft(posCollider, sizeCollider)) {
			direction = DOWN_LEFT;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;
		}
		//No tenim terra a sota
		posEnemy.y -= 2;
		posCollider.y = posEnemy.y + 2;
		//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
		bool isGrounded = map->collisionMoveUp(posCollider, sizeCollider, &posEnemy.y);
		//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;
		posCollider.y = posEnemy.y + 2;
		if (!isGrounded) {
			//cout << "It's not grounded" << endl;
			direction = UP_RIGHT;
			posEnemy.x += sizeHorizontal.x - sizeVertical.x;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;

		}
	}
	//ens estem movent a la dreta cap per avall
	else if (direction == RIGHT_UPSIDE) {
		//Tenim un mur a l'esquerra
		if (map->collisionMoveRight(posCollider, sizeCollider)) {
			direction = DOWN_RIGHT;
			posEnemy.x += sizeHorizontal.x - sizeVertical.x;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;
		}
		//No tenim terra a sota
		posEnemy.y -= 2;
		posCollider.y = posEnemy.y + 2;
		//cout << "Enemy and collider y before: " << posEnemy.y << ' ' << posCollider.y << endl;
		bool isGrounded = map->collisionMoveUp(posCollider, sizeCollider, &posEnemy.y);
		//cout << "Enemy and collider y after: " << posEnemy.y << ' ' << posCollider.y << endl;
		posCollider.y = posEnemy.y + 2;
		if (!isGrounded) {
			//cout << "It's not grounded" << endl;
			direction = UP_LEFT;
			sizeEnemy = sizeVertical;
			sizeCollider = sizeColliderVertical;
			movingVertical = true;

		}
	}
	//cout << "Direction is: " << direction << endl;

	//cout << "Direction is: " << direction << endl;
	selectAnim();
}

void EnemySnail::selectAnim() {
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

	//cout << "Animation is: " << new_anim << endl;

	if (new_anim != sprite->animation())
		sprite->changeAnimation(new_anim);
}

void EnemySnail::handleHiding(int deltaTime)
{
	if (!hiding) {
		timeToHide -= deltaTime;
		if (timeToHide < 0) {
			int num = rand() % 100;
			//cout << "END_TIME" << endl;
			if (num < HIDE_PROBABILITY) {
				hide();
				hideAnimationDuration = HIDE_ANIMATION_DURATION;
			}
			else timeToHide = MIN_TIME_HIDE;
		}
	}
	else {
		handleHideAnimation(deltaTime);
		hideTimeRemaining -= deltaTime;
		if (hideTimeRemaining < 0) {
			int num = rand() % 100;
			//cout << "END_TIME" << endl;
			if (num < END_HIDE_PROBABILITY) {
				unhide();
			}
			else hideTimeRemaining = MIN_DURATION_HIDE;
		}
	}
}

void EnemySnail::hide()
{
	hiding = true;
	hideTimeRemaining = MIN_DURATION_HIDE;
}

void EnemySnail::unhide()
{
	hiding = false;
	timeToHide = MIN_TIME_HIDE;
}

void EnemySnail::handleHideAnimation(int deltaTime)
{
	int new_anim = sprite->animation();

	if (hideAnimationDuration > 0) {
		hideAnimationDuration -= deltaTime;
		switch (direction)
		{
			case LEFT:
				new_anim = HIDE_LEFT;
				break;
			case RIGHT:
				new_anim = HIDE_RIGHT;
				break;
			case UP_LEFT:
				new_anim = HIDE_UP_LEFT_SIDE;
				break;
			case UP_RIGHT:
				new_anim = HIDE_UP_RIGHT_SIDE;
				break;
			case DOWN_LEFT:
				new_anim = HIDE_DOWN_LEFT_SIDE;
				break;
			case DOWN_RIGHT:
				new_anim = HIDE_DOWN_RIGHT_SIDE;
				break;
			case LEFT_UPSIDE:
				new_anim = HIDE_LEFT_UPSIDE;
				break;
			case RIGHT_UPSIDE:
				new_anim = HIDE_RIGHT_UPSIDE;
				break;
			default:
				break;
		}
	}
	else {
		//cout << "TIME is 0" << endl;
		switch (direction)
		{
			case LEFT:
				new_anim = HIDEN_LEFT;
				break;
			case RIGHT:
				new_anim = HIDEN_RIGHT;
				break;
			case UP_LEFT:
				new_anim = HIDEN_UP_LEFT_SIDE;
				break;
			case UP_RIGHT:
				new_anim = HIDEN_UP_RIGHT_SIDE;
				break;
			case DOWN_LEFT:
				new_anim = HIDEN_DOWN_LEFT_SIDE;
				break;
			case DOWN_RIGHT:
				new_anim = HIDEN_DOWN_RIGHT_SIDE;
				break;
			case LEFT_UPSIDE:
				new_anim = HIDEN_LEFT_UPSIDE;
				break;
			case RIGHT_UPSIDE:
				new_anim = HIDEN_RIGHT_UPSIDE;
				break;
			default:
				break;
		}
	}

	//cout << "Animation is: " << new_anim << endl;

	if (new_anim != sprite->animation())
		sprite->changeAnimation(new_anim);
}

void EnemySnail::getHurt(int damage) {
	if(!hiding) Enemy::getHurt(damage);
}


