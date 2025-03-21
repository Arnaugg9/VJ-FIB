#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bullet.h"
#include "Game.h"

enum anims {
	MOVING_LEFT, MOVING_RIGHT
};

enum Directions
{
	LEFT, RIGHT
};

void Bullet::init(const glm::ivec2 size, int dir, Sprite* bulletSprite, int speed, int timeToLive)
{
	this->size = size;
	sizeCollider = size;
	sizeCollider.x -= 2;
	sizeCollider.y -= 4;

	direction = dir;
	sprite = bulletSprite;
	this->speed = speed;

	this->timeToLive = timeToLive;
	active = false;
}

void Bullet::update(int deltaTime)
{
	if (timeRest > 0) {
		pos.x += direction == LEFT ? -speed : speed;
		timeRest -= deltaTime;
		sprite->setPosition(pos);
	}
	else deactivate();

	posCollider = pos;
	posCollider.x += 1;
	posCollider.y += 2;
}

void Bullet::render()
{
	sprite->render();
}

glm::ivec2 Bullet::getPosition()
{
	return pos;
}

glm::ivec2 Bullet::getSize()
{
	return size;
}

glm::ivec2 Bullet::getColliderPosition()
{
	return posCollider;
}

glm::ivec2 Bullet::getColliderSize()
{
	return sizeCollider;
}

bool Bullet::isActive()
{
	return active;
}

void Bullet::activate(glm::ivec2& posBullet, int dir)
{
	pos = posBullet;
	posCollider = pos;
	posCollider.x += 1;
	posCollider.y += 2;
	active = true;

	direction = dir;

	int anim = direction == LEFT ? MOVING_LEFT : MOVING_RIGHT;
	sprite->changeAnimation(anim);
	sprite->setPosition(pos);
	timeRest = timeToLive;
}

void Bullet::deactivate()
{
	active = false;
	pos = glm::ivec2(0);
}
