#ifndef _ENEMY_FLOWER_INCLUDE
#define _ENEMY_FLOWER_INCLUDE


#include "Enemy.h"

class EnemyFlower : public Enemy
{

public:
	void init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	bool getHurt(int damage);

private:
	glm::vec2 initialBulletPos, posBullet, sizeBullet;
	int timeShoot;		//Temps que dura una bullet
	int timeToShoot;	//Temps entre disparos (temps entre que acaba timeShoot a que comença altra vegada)

	Sprite* spriteBullet;
};

#endif // _ENEMY_FLOWER_INCLUDE