#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 64
#define EXTRA_JUMP_HEIGHT 16
#define FALL_STEP 4

#define ATTACK_TIME 400

#define INITIAL_MAX_HEALTH 12
#define INITIAL_LIVES 1

#define MAX_INVENCIBILITY_TIME 1500
#define FRAMES_AUX_HURT_ANIMATION 5
#define TIME_HURT_ANIMATION 500
#define PLAYER_DAMAGE 1;

#define LEFT_BOSSFIGHT 48*16

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, ATCK_LEFT, ATCK_RIGHT, ATCK_MOVE_LEFT, ATCK_MOVE_RIGHT,
	ATCK_JUMP_LEFT, ATCK_JUMP_RIGHT, COVER_LEFT, COVER_RIGHT, ATCK_JUMP_UP_LEFT, ATCK_JUMP_UP_RIGHT,
	ATCK_JUMP_DOWN_LEFT, ATCK_JUMP_DOWN_RIGHT, HURT_LEFT, HURT_RIGHT
};

enum SwordAnims
{
	SWORD_LEFT, SWORD_RIGHT
};

enum Directions
{
	LEFT, RIGHT
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	playerColliderSize = glm::ivec2(18.f, 32.f);

	sizePlayer = glm::ivec2(24, 32);

	bJumping = false;
	dirPlayer = RIGHT;

	isAttacking = false;
	isAttacking2 = false;
	isGrounded = true;
	isCovering = false;
	isCrouching = false;
	previousJumpState = false;
	onBossfight = false;

	defensiveHits = 0;
	attackingHits = 0;

	attackTime = ATTACK_TIME;
	playerDamage = PLAYER_DAMAGE;

	maxHealth = health = INITIAL_MAX_HEALTH;	//Al principi té 4 cors, cada unitat es 1 terç de cor
	lives = INITIAL_LIVES;
	invencibilityTime = 0;
	auxAnimationHurt = FRAMES_AUX_HURT_ANIMATION;

	spritesheet.loadFromFile("images/playerSprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
		//Cada sprite ocupa 0.25 en x y 0.0625 en y
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.25, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);
	
		//STAND_LEFT
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.0625f));

		//STAND_RIGHT
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));


		//MOVE_LEFT
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.0625f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.0625f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.0625f));
		
		//MOVE_RIGHT
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.f));


		//JUMP_LEFT
		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.1875f));

		//JUMP_RIGHT
		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.f, 0.1875f));


		//ATTACK_LEFT
		sprite->setAnimationSpeed(ATCK_LEFT, 8);
		sprite->addKeyframe(ATCK_LEFT, glm::vec2(0.f, 0.375f));

		//ATTACK_RIGHT
		sprite->setAnimationSpeed(ATCK_RIGHT, 8);
		sprite->addKeyframe(ATCK_RIGHT, glm::vec2(0.f, 0.3125f));

		//ATTACK_MOVE_LEFT
		sprite->setAnimationSpeed(ATCK_MOVE_LEFT, 8);
		sprite->addKeyframe(ATCK_MOVE_LEFT, glm::vec2(0.f, 0.375f));
		sprite->addKeyframe(ATCK_MOVE_LEFT, glm::vec2(0.25f, 0.375f));
		sprite->addKeyframe(ATCK_MOVE_LEFT, glm::vec2(0.5f, 0.375f));
		sprite->addKeyframe(ATCK_MOVE_LEFT, glm::vec2(0.75f, 0.375f));

		//ATTACK_MOVE_RIGHT
		sprite->setAnimationSpeed(ATCK_MOVE_RIGHT, 8);
		sprite->addKeyframe(ATCK_MOVE_RIGHT, glm::vec2(0.f, 0.3125f));
		sprite->addKeyframe(ATCK_MOVE_RIGHT, glm::vec2(0.25f, 0.3125f));
		sprite->addKeyframe(ATCK_MOVE_RIGHT, glm::vec2(0.5f, 0.3125f));
		sprite->addKeyframe(ATCK_MOVE_RIGHT, glm::vec2(0.75f, 0.3125f));

		//ATTACK_STAND_LEFT
		sprite->setAnimationSpeed(ATCK_JUMP_LEFT, 8);
		sprite->addKeyframe(ATCK_JUMP_LEFT, glm::vec2(0.75f, 0.1875f));

		//ATTACK_JUMP_RIGHT
		sprite->setAnimationSpeed(ATCK_JUMP_RIGHT, 8);
		sprite->addKeyframe(ATCK_JUMP_RIGHT, glm::vec2(0.25f, 0.1875f));


		//COVER_LEFT
		sprite->setAnimationSpeed(COVER_LEFT, 8);
		sprite->addKeyframe(COVER_LEFT, glm::vec2(0.25f, 0.4375f));

		//COVER_RIGHT
		sprite->setAnimationSpeed(COVER_RIGHT, 8);
		sprite->addKeyframe(COVER_RIGHT, glm::vec2(0.f, 0.4375f));


		//ATTACK_JUMP_UP_LEFT
		sprite->setAnimationSpeed(ATCK_JUMP_UP_LEFT, 8);
		sprite->addKeyframe(ATCK_JUMP_UP_LEFT, glm::vec2(0.5f, 0.25f));

		//ATTACK_JUMP_UP_RIGHT
		sprite->setAnimationSpeed(ATCK_JUMP_UP_RIGHT, 8);
		sprite->addKeyframe(ATCK_JUMP_UP_RIGHT, glm::vec2(0.f, 0.25f));

		//ATTACK_JUMP_DOWN_LEFT
		sprite->setAnimationSpeed(ATCK_JUMP_DOWN_LEFT, 8);
		sprite->addKeyframe(ATCK_JUMP_DOWN_LEFT, glm::vec2(0.75f, 0.25f));

		//ATTACK_JUMP_DOWN_RIGHT
		sprite->setAnimationSpeed(ATCK_JUMP_DOWN_RIGHT, 8);
		sprite->addKeyframe(ATCK_JUMP_DOWN_RIGHT, glm::vec2(0.25f, 0.25f));


		//HURT_LEFT
		sprite->setAnimationSpeed(HURT_LEFT, 8);
		sprite->addKeyframe(HURT_LEFT, glm::vec2(0.25f, 0.5f));

		//HURT_RIGHT
		sprite->setAnimationSpeed(HURT_RIGHT, 8);
		sprite->addKeyframe(HURT_RIGHT, glm::vec2(0.f, 0.5f));
		
	sprite->changeAnimation(STAND_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(posPlayer);

	
	swordSprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.25, 0.0625), &spritesheet, &shaderProgram);
	swordSprite->setNumberAnimations(2);

		//SWORD_LEFT
		swordSprite->setAnimationSpeed(SWORD_LEFT, 18);
		swordSprite->addKeyframe(SWORD_LEFT, glm::vec2(0.f, 0.6875f));
		swordSprite->addKeyframe(SWORD_LEFT, glm::vec2(0.25f, 0.6875f));
		swordSprite->addKeyframe(SWORD_LEFT, glm::vec2(0.5f, 0.6875f));
		swordSprite->addKeyframe(SWORD_LEFT, glm::vec2(0.75f, 0.6875f));
		swordSprite->addKeyframe(SWORD_LEFT, glm::vec2(0.5f, 0.6875f));
		swordSprite->addKeyframe(SWORD_LEFT, glm::vec2(0.25f, 0.6875f));


		//SWORD_RIGHT
		swordSprite->setAnimationSpeed(SWORD_RIGHT, 18);
		swordSprite->addKeyframe(SWORD_RIGHT, glm::vec2(0.f, 0.625f));
		swordSprite->addKeyframe(SWORD_RIGHT, glm::vec2(0.25f, 0.625f));
		swordSprite->addKeyframe(SWORD_RIGHT, glm::vec2(0.5f, 0.625f));
		swordSprite->addKeyframe(SWORD_RIGHT, glm::vec2(0.75f, 0.625f));
		swordSprite->addKeyframe(SWORD_RIGHT, glm::vec2(0.5f, 0.625f));
		swordSprite->addKeyframe(SWORD_RIGHT, glm::vec2(0.25f, 0.625f));



		swordSprite->changeAnimation(SWORD_RIGHT);
		swordSprite->setPosition(glm::ivec2(posPlayer.x + 28, posPlayer.y));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (isAttacking) swordSprite->update(deltaTime);
	
	int tileID = map->collisionSpecialTile(posPlayerCollision, playerColliderSize);
	if (invencibilityTime <= 0 && timeHurtAnimation <= 0 && tileID == 43) this->getHurt(1);

	handleInvincibility(deltaTime);
	handleMovement();
	if (timeHurtAnimation <= 0) handleJump();
	handleAttack(deltaTime);
	isAttacking2 = (isAttacking || (!isGrounded && (isCovering || isCrouching)));
	if (timeHurtAnimation <= 0 && (!isCrouching || !isGrounded)) sprite->setPosition(posPlayer);
	else if (timeHurtAnimation <= 0) sprite->setPosition(glm::vec2(posPlayer.x, posPlayer.y + 8.f));
	posPlayerCollision = glm::ivec2(posPlayer.x + 7, posPlayer.y);

	handleSwordSprite();
}

void Player::handleMovement()
{
	int moveSpeed = 2;
	bool movingLeft = Game::instance().getKey(GLFW_KEY_LEFT);
	bool movingRight = Game::instance().getKey(GLFW_KEY_RIGHT);
	
	//Comrova si ens ajupim o en cobrim (si es fan les dues no es fa cap)
	isCovering = Game::instance().getKey(GLFW_KEY_UP);
	isCrouching = Game::instance().getKey(GLFW_KEY_DOWN);
	if (isCovering && isCrouching) isCovering = isCrouching = false;

	//Mira si es mou a algun costat
	if (movingLeft ^ movingRight) {

		//Calcula següent posició
		dirPlayer = movingLeft ? LEFT : RIGHT;
		posPlayer.x += movingLeft ? -moveSpeed : moveSpeed;
		posPlayerCollision.x = posPlayer.x+7;

		//Controla colisions als costats i que si ens cobrim/ajupim (i no saltem) no ens podem moure
		if ((movingLeft && map->collisionMoveLeft(posPlayerCollision, playerColliderSize)) ||
			(movingRight && map->collisionMoveRight(posPlayerCollision, playerColliderSize)) ||
			(isCovering && isGrounded) ||
			(isCrouching && isGrounded) ||
			(onBossfight && posPlayer.x < LEFT_BOSSFIGHT-5)) {
			//Si hi ha colisió o ens estem cobrint/ajupint (i no saltant) es queda idle
			posPlayer.x += movingLeft ? moveSpeed : -moveSpeed;
			posPlayerCollision.x = posPlayer.x + 7;
			handleIdle();
			return;
		}

		int new_anim = 0;

		//Si ens movem i saltem
		if (!isGrounded) {
			//escull en funció de si també s'ataca o no
			if (isCovering) new_anim = movingLeft ? ATCK_JUMP_UP_LEFT : ATCK_JUMP_UP_RIGHT;
			else if (isCrouching) new_anim = movingLeft ? ATCK_JUMP_DOWN_LEFT : ATCK_JUMP_DOWN_RIGHT;
			else if (isAttacking) new_anim = movingLeft ? ATCK_JUMP_LEFT : ATCK_JUMP_RIGHT;
			else new_anim = movingLeft ? JUMP_LEFT : JUMP_RIGHT;
		}
		//Si ens movem i ataquem
		else if (isAttacking) 
			new_anim = movingLeft ? ATCK_MOVE_LEFT : ATCK_MOVE_RIGHT;
		//Si ens movem només
		else
			new_anim = movingLeft ? MOVE_LEFT : MOVE_RIGHT;

		//Executa l'animació seleccionada
		if (new_anim != sprite->animation() && timeHurtAnimation <= 0) sprite->changeAnimation(new_anim);
	}
	else {
		handleIdle();
	}
}

void Player::handleIdle()
{
	int new_anim = 0;

	if (!isGrounded) {
		if (isCrouching) new_anim = dirPlayer == LEFT ? ATCK_JUMP_DOWN_LEFT : ATCK_JUMP_DOWN_RIGHT;
		else if (isCovering) new_anim = dirPlayer == LEFT ? ATCK_JUMP_UP_LEFT : ATCK_JUMP_UP_RIGHT;
		else if (isAttacking) new_anim = dirPlayer == LEFT ? ATCK_JUMP_LEFT : ATCK_JUMP_RIGHT;
		else new_anim = dirPlayer == LEFT ? JUMP_LEFT : JUMP_RIGHT;
	}
	else if (isCrouching) {
		if (isAttacking) new_anim = dirPlayer == LEFT ? ATCK_JUMP_LEFT : ATCK_JUMP_RIGHT;
		else new_anim = dirPlayer == LEFT ? JUMP_LEFT : JUMP_RIGHT;
	}
	else if (isAttacking)
		new_anim = dirPlayer == LEFT ? ATCK_LEFT : ATCK_RIGHT;
	else if (isCovering) 
		new_anim = dirPlayer == LEFT ? COVER_LEFT : COVER_RIGHT;
	else
		new_anim = dirPlayer == LEFT ? STAND_LEFT : STAND_RIGHT;

	if (new_anim != sprite->animation() && timeHurtAnimation <= 0) sprite->changeAnimation(new_anim);
}

void Player::handleJump()
{
	bool jumpKeyPressed = Game::instance().getKey(GLFW_KEY_Z);
	if (bJumping) {
		isGrounded = false;
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180) {
			bJumping = false;
			posPlayer.y = startY;
		}
		else {
			int currentJumpHeight = JUMP_HEIGHT;
			if (extraJump) currentJumpHeight += EXTRA_JUMP_HEIGHT;
			posPlayer.y = int(startY - currentJumpHeight * sin(3.14159f * jumpAngle / 180.f));
			//cout << currentJumpHeight << endl;
			if (jumpAngle > 90) {
				posPlayerCollision.y = posPlayer.y;
				bJumping = !map->collisionMoveDown(posPlayerCollision, playerColliderSize, &posPlayer.y);
				posPlayerCollision.y = posPlayer.y;
			}
		}
	}
	else {
		posPlayer.y += FALL_STEP;
		posPlayerCollision.y = posPlayer.y;
		isGrounded = map->collisionMoveDown(posPlayerCollision, playerColliderSize, &posPlayer.y);
		posPlayerCollision.y = posPlayer.y;
		if (isGrounded) {
			int tileID = map->collisionSpecialTile(posPlayerCollision, playerColliderSize);
			extraJump = (tileID == 47 || tileID == 48 || tileID == 57 || tileID == 58);

			if (jumpKeyPressed && !previousJumpState) {
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	previousJumpState = jumpKeyPressed;
}

void Player::handleAttack(int deltaTime)
{
	bool attackKeyPressed = Game::instance().getKey(GLFW_KEY_X);
	if (isAttacking) {
		attackTime -= deltaTime;
		if (attackTime <= 0) {
			isAttacking = false;
			attackTime = ATTACK_TIME;
			swordSprite->changeAnimation(swordSprite->animation());
		}
	}
	else if (attackKeyPressed && !previousAttackState && (isGrounded || (!isCovering && !isCrouching))) {
		isAttacking = true;
	}
	previousAttackState = attackKeyPressed;
}

void Player::handleSwordSprite()
{
	int new_anim = dirPlayer == LEFT ? SWORD_LEFT : SWORD_RIGHT;

	if (new_anim != swordSprite->animation()) swordSprite->changeAnimation(new_anim);

	if (isGrounded && isCrouching) swordSprite->setPosition(glm::ivec2(posPlayer.x + (dirPlayer == LEFT ? -28 : 28), posPlayer.y + 8));
	else swordSprite->setPosition(glm::ivec2(posPlayer.x + (dirPlayer == LEFT ? -28 : 28), posPlayer.y));
}

void Player::getHurt(int damage) {
	if (defensiveHits > 0) {
		--defensiveHits;
		health -= damage / 2;
	}  
	else health -= damage;
	invencibilityTime = MAX_INVENCIBILITY_TIME;
	timeHurtAnimation = TIME_HURT_ANIMATION;
	sprite->changeAnimation(dirPlayer == LEFT ? HURT_LEFT : HURT_RIGHT);
	sprite->setPosition(glm::vec2(posPlayer.x, posPlayer.y - (timeHurtAnimation * 0.125)/2.f));
	cout << "HEALTH: " << health << endl;
}

void Player::handleInvincibility(int deltaTime) {
	if (invencibilityTime > 0) {
		if (timeHurtAnimation > 0) {
			timeHurtAnimation -= deltaTime;
			sprite->setPosition(glm::vec2(posPlayer.x, posPlayer.y - timeHurtAnimation * 0.125));
			sprite->changeAnimation(dirPlayer == LEFT ? HURT_LEFT : HURT_RIGHT);
		}
		else {
			invencibilityTime -= deltaTime;
		}
	}
}

void Player::render()
{
	if (invencibilityTime > 0 && timeHurtAnimation <= 0) {
		if (auxAnimationHurt <= FRAMES_AUX_HURT_ANIMATION / 2) {
			sprite->render();
			if (isAttacking) swordSprite->render();
		}
		if (auxAnimationHurt <= 0) {
			auxAnimationHurt = FRAMES_AUX_HURT_ANIMATION;
		}
		--auxAnimationHurt;
	}
	else {
		sprite->render();
		if (isAttacking && timeHurtAnimation <= 0) swordSprite->render();
	}

}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	posPlayerCollision = glm::ivec2(posPlayer.x + 7, posPlayer.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::setOnBossfight(bool value)
{
	onBossfight = value;
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

glm::ivec2 Player::getColliderPosition() {
	if (!isAttacking2)
		return posPlayerCollision;

	else if (!isGrounded && isCovering)
		return posPlayerCollision;

	else if (!isGrounded && isCrouching)
		return glm::ivec2(posPlayerCollision.x, posPlayerCollision.y + 19);

	else if (dirPlayer == LEFT)
		return glm::ivec2(posPlayerCollision.x - 32, posPlayerCollision.y);

	else if (dirPlayer == RIGHT)
		return glm::ivec2(posPlayerCollision.x + playerColliderSize.x, posPlayerCollision.y);

	return posPlayerCollision;
}

glm::ivec2 Player::getColliderSize() {
	if (!isAttacking2)
		return playerColliderSize;

	else if (!isGrounded && isCovering)
		return glm::ivec2(playerColliderSize.x, 15);

	else if (!isGrounded && isCrouching)
		return glm::ivec2(playerColliderSize.x, 17);

	else if (dirPlayer == LEFT)
		return glm::ivec2(32, playerColliderSize.y);

	else if (dirPlayer == RIGHT)
		return glm::ivec2(34, playerColliderSize.y);

	return playerColliderSize;
}

glm::ivec2 Player::getColliderPositionNeutral()
{
	return posPlayerCollision;
}

glm::ivec2 Player::getColliderSizeNeutral()
{
	return playerColliderSize;
}

bool Player::isInvencible() {
	return invencibilityTime > 0;
}

void Player::updateHealth(int health) {
	if (health == -1) this->health = maxHealth;
	else if (this->health + health <= maxHealth) this->health += health;
	else this->health = maxHealth;
}

void Player::updateGourds() {
	++gourds;
	if (gourds == 9) {
		maxHealth += 3;
		health += 3;
	}
	else if (gourds == 12) {
		maxHealth += 3;
		health += 3;
	}
	else if (gourds == 16) {
		maxHealth += 3;
		health += 3;
	}
	else if (gourds == 22) {
		maxHealth += 3;
		health += 3;
	}
	else if (gourds == 30) {
		maxHealth += 3;
		health += 3;
	}
	else if (gourds == 42) {
		maxHealth += 3;
		health += 3;
	}
	else if (gourds == 62) {
		maxHealth += 3;
		health += 3;
	}
	else if (gourds == 99) {
		maxHealth += 3;
		health += 3;
	}
}

void Player::setInvencible() {
	invencibilityTime = 5000;
}

void Player::setDefensiveHits(int hits) {
	defensiveHits = hits;
}

void Player::setAttackingHits(int hits) {
	attackingHits = hits;
}
void Player::healCheat()
{
	health = maxHealth;
	lives = 2;
}
bool Player::isOnBossfight()
{
	return onBossfight;
}

bool Player::getIsAttacking2()
{
	return isAttacking2;
}

int Player::getDamage()
{
	return playerDamage;
}

