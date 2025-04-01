#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getPosition();
	glm::ivec2 getColliderPosition();
	glm::ivec2 getColliderSize();

	void getHurt(int damage);
	bool isInvencible();
	void updateHealth(int health);
	void updateGourds();
	void setInvencible();
	void setDefensiveHits(int hits);
	void setAttackingHits(int hits);
	
private:
	//Controladors de moviment
	int dirPlayer;
	bool bJumping;
	glm::ivec2 playerColliderSize;

	//Triggers per comprovacions
	bool isGrounded;
	bool isAttacking;
	bool isCovering;
	bool isCrouching;

	bool previousJumpState;		//Control de que només conti apretar la tecla i no mantenir apretat
	bool previousAttackState;

	//Temps que dura l'animacio d'atac (en ms i no frames per evitar problemes de fps)
	int attackTime;

	//Posicions i colisions
	glm::ivec2 tileMapDispl, posPlayer;
	glm::ivec2 posPlayerCollision;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	//Caracterisitiques personatge
	int health;
	int maxHealth;
	int gourds;
	int invencibilityTime; //Controla la inmortalitat del personatge --> Està en ms (per evitar problemes de diferents fps...)
	int auxAnimationHurt;
	int timeHurtAnimation;

	bool extraJump;

	int defensiveHits;
	int attackingHits;

	void handleMovement();
	void handleIdle();
	void handleJump();
	void handleInvincibility(int deltaTime);
	void handleAttack(int deltaTime);
};


#endif // _PLAYER_INCLUDE


