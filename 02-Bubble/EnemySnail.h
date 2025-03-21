#ifndef _ENEMY_SNAIL_INCLUDE
#define _ENEMY_SNAIL_INCLUDE

#include "Enemy.h"

class EnemySnail : public Enemy
{
public:
	void init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	Bullet* getBullet();

private:
	void handleMovement();
	void moveV();
	void moveH();
	void hide();

	bool hiding;

};

#endif // _ENEMY_SNAIL_INCLUDE

