#ifndef _ENEMY_SNAIL_INCLUDE
#define _ENEMY_SNAIL_INCLUDE

#include "Enemy.h"

class EnemySnail : public Enemy
{
public:
	void init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram, int dir);
	void update(int deltaTime);
	void render();

	bool getHurt(int damage);

private:
	void handleMovement(int deltaTime);
	void moveV(int deltaTime);
	void moveH(int deltaTime);
	void handleHiding(int deltaTime);
	void hide();
	void unhide();
	void handleHideAnimation(int deltaTime);

	void selectAnim();

	bool hiding;
	bool movingVertical;
	
	int hideTimeRemaining;	//Controla temps de hide
	int timeToHide;			//Controla cada quant temps es comprova si s'amaga
	int hideAnimationDuration;

	int auxSlowSpeed;

	glm::ivec2 sizeHorizontal;
	glm::ivec2 sizeVertical;
	glm::ivec2 sizeColliderHorizontal;
	glm::ivec2 sizeColliderVertical;

};

#endif // _ENEMY_SNAIL_INCLUDE

