#ifndef _ENEMY_SNAIL_INCLUDE
#define _ENEMY_SNAIL_INCLUDE

#include "Enemy.h"

class EnemySnail : public Enemy
{
public:
	void init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

private:
	void handleMovement(int deltaTime);
	void moveV(int deltaTime);
	void moveH(int deltaTime);
	void handleHiding(int deltaTime);
	void hide();

	bool hiding;
	bool movingVertical;

	glm::ivec2 sizeHorizontal;
	glm::ivec2 sizeVertical;
	glm::ivec2 sizeColliderHorizontal;
	glm::ivec2 sizeColliderVertical;

};

#endif // _ENEMY_SNAIL_INCLUDE

