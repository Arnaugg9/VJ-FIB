#ifndef _ENEMY_ELEPHANT_INCLUDE
#define _ENEMY_ELEPHANT_INCLUDE

#include "Enemy.h"

class EnemyElephant : public Enemy
{
public:
	void init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	Bullet* getBullet();

private:
	void move();
	void jump();
	void handleShoot(int deltaTime);
	void shoot();

	bool bJumping;
	int jumpAngle, startY, jumpHeight;
	Bullet *bullet;
	Sprite* bulletSprite;
	int timeToShoot;
};

#endif // _ENEMY_ELEPHANT_INCLUDE