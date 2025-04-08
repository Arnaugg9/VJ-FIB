#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "EnemyFlower.h"
#include "Game.h"
#include "SoundManager.h"

#define MAX_TIME_SHOOT 750
#define MAX_TIME_TO_SHOOT 500

void EnemyFlower::init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram)
{
	Enemy::init(enemyPos, shaderProgram);

	sizeEnemy = glm::ivec2(16.f, 32.f);

	health = 10;
	
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.3125f));
	sprite->addKeyframe(0, glm::vec2(0.125f, 0.3125f));

	sprite->changeAnimation(0);
	sprite->setPosition(posEnemy);

	//init de la bala
	initialBulletPos = posEnemy;
	initialBulletPos.x += 5;
	posBullet = initialBulletPos;

	//La flor no té colisions així que per "aprofitar codi" utilitzarem el collider per la bullet (enlloc de fer la bala una entitat separada)
	damage = 1;

	posCollider = glm::ivec2(0, 0);			//Quan no esta disparant no volem "colisionar" --> Aprofitem les colisions de Enemy
	sizeCollider = glm::ivec2(6, 11);

	timeToShoot = MAX_TIME_TO_SHOOT;
	timeShoot = 0;

	spriteBullet = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.0625), &spritesheet, &shaderProgram);
	spriteBullet->setNumberAnimations(1);

	spriteBullet->setAnimationSpeed(0, 8);
	spriteBullet->addKeyframe(0, glm::vec2(0.25f, 0.3125f));

	spriteBullet->changeAnimation(0);
	spriteBullet->setPosition(posBullet);
}

void EnemyFlower::update(int deltaTime)
{
	Enemy::update(deltaTime);

	//Hi ha bala
	if (timeShoot > 0) {
		timeToShoot = MAX_TIME_TO_SHOOT;
		timeShoot -= deltaTime;
		posBullet.y -= 3;
		posCollider.y -= 3;
	}
	//Hem de disparar
	else if (timeToShoot <= 0) {
		timeShoot = MAX_TIME_SHOOT;
		posCollider = initialBulletPos;
		SoundManager::playSFX("sounds/effects/bullet.wav");
	}
	//Encara no hem de disparar
	else {
		posCollider = glm::ivec2(0, 0);
		posBullet = initialBulletPos;
		timeToShoot -= deltaTime;
	}
	spriteBullet->setPosition(posBullet);
}

void EnemyFlower::render()
{
	Enemy::render();

	if (timeShoot > 0) spriteBullet->render();
}

bool EnemyFlower::getHurt(int damage)
{
	return false;
}