#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Enemy.h"

class Boss : public Enemy
{
public:
	void init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	Bullet* getBullet(int numBullet);
	bool getHurt(int damage);

private:
	void phase0Control(int deltaTime);
	void changePhase(int deltaTime);
	void phase1Control(int deltaTime);

	Bullet* bullets[4];
	Sprite* bulletsSprites[4];

	Sprite* stringSprites[10];

	glm::ivec2 sizePhase1;
	glm::ivec2 sizeColliderPhase1;

	int phaseNumber;
	
	//Parametres fase 0
	int timeToRise;
	int timeToFall;
	bool falling;
	bool rising;

	bool changingPhase;
	int phaseChangeTimer;

	//Parametres fase 1
	bool inDestination;
	int timerAttackDuration;
};

#endif // _BOSS_INCLUDE