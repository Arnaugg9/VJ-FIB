#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Enemy.h"
#include "Player.h"

class Boss : public Enemy
{
public:
	void init(const glm::ivec2& enemyPos, Player* player, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	Bullet* getBullet(int numBullet);
	bool getHurt(int damage);
	void dieAnimation(int deltaTime);

private:
	void phase0Control(int deltaTime);
	void changePhase(int deltaTime);
	void phase1Control(int deltaTime);

	void moveTo(int deltaTime, glm::ivec2 dest);
	void handleAttack(int deltaTime);

	Player* player;

	Bullet* bullets[4];
	glm::ivec2 posInitBullets[4];
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
	glm::ivec2 destination;
	bool attacked;

	//Parametres animacio mort
};

#endif // _BOSS_INCLUDE