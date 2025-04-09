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
	void setOnBossfight(bool value);

	glm::ivec2 getPosition();
	glm::ivec2 getColliderPosition();
	glm::ivec2 getColliderSize();
	glm::ivec2 getColliderPositionNeutral();
	glm::ivec2 getColliderSizeNeutral();

	void getHurt(int damage);
	bool isInvencible();
	bool isOnBossfight();

	bool getIsAttacking2();
	int getDamage();

	void endAnimation(int deltaTime);
	
	void updateHealth(int health);
	void updateGourds();
	void setInvencible();
	void setDefensiveHits(int hits);
	void setAttackingHits(int hits);
	void setWeapon(int type);
	int getDefensiveHits() { return defensiveHits; }
	int getAttackingHits() { return attackingHits; }
	int getHealth() { return health; }
	int getMaxHealth() { return maxHealth; }
	int getLives() { return lives; }
	bool getDie() { return die; };
	
	void healCheat();
	bool godModeOn;

private:
	//Controladors de moviment
	int dirPlayer;
	bool bJumping;
	glm::ivec2 playerColliderSize;

	//Triggers per comprovacions
	bool isGrounded;
	bool isAttacking;
	bool isAttacking2;			//Es un apaño per controlar el atac
	bool isCovering;
	bool isCrouching;
	bool onBossfight;

	bool previousJumpState;		//Control de que només conti apretar la tecla i no mantenir apretat
	bool previousAttackState;

	//Temps que dura l'animacio d'atac (en ms i no frames per evitar problemes de fps)
	int attackTime;
	int playerDamage;

	//Posicions i colisions
	glm::ivec2 tileMapDispl, posPlayer, sizePlayer;
	glm::ivec2 posPlayerCollision;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	Sprite *swordSprite;
	int weaponType;

	//Caracterisitiques personatge
	int health;
	int maxHealth;
	int lives;
	int gourds;
	int invencibilityTime; //Controla la inmortalitat del personatge --> Està en ms (per evitar problemes de diferents fps...)
	int auxAnimationHurt;
	int timeHurtAnimation;
	bool die;
	bool sound;

	bool extraJump;

	int defensiveHits;
	int attackingHits;

	void handleMovement();
	void handleIdle();
	void handleJump();
	void handleInvincibility(int deltaTime);
	void handleAttack(int deltaTime);

	void handleSwordSprite();
};


#endif // _PLAYER_INCLUDE


