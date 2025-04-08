#include <iostream>
#include <GL/glew.h>
#include "UI.h"
#include "Game.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::init(const glm::ivec2& uiPos, ShaderProgram& shaderProgram)
{
	this->shaderProgram = &shaderProgram;

    spritesheetUI.loadFromFile("images/UI_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheetUI.setMinFilter(GL_NEAREST);
    spritesheetUI.setMagFilter(GL_NEAREST);
	bossfight = false;

    spritesheet.loadFromFile("images/UI_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

	spritesheetBoss.loadFromFile("images/spirtesheet_boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetBoss.setMinFilter(GL_NEAREST);
	spritesheetBoss.setMagFilter(GL_NEAREST);

    spear = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    spear->setNumberAnimations(1);
    spear->setAnimationSpeed(0, 8);
    spear->addKeyframe(0, glm::vec2(0.0f, 0.33333333333f));
    spear->changeAnimation(0);
    spear->setPosition(uiPos);

    fire = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheetUI, &shaderProgram);
    fire->setNumberAnimations(1);
    fire->setAnimationSpeed(0, 8);
    fire->addKeyframe(0, glm::vec2(0.33333333333f, 0.33333333333f));
    fire->changeAnimation(0);
    fire->setPosition(uiPos);

    num_hearts = 4;

	for (int i = 0; i < 12; ++i) {
		hearts[i] = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheetUI, &shaderProgram);
		hearts[i]->setNumberAnimations(4);

		hearts[i]->setAnimationSpeed(0, 8);
		hearts[i]->addKeyframe(0, glm::vec2(0.0f, 0.0f));

		hearts[i]->setAnimationSpeed(1, 8);
		hearts[i]->addKeyframe(1, glm::vec2(0.33333333333f, 0.0f));
		
		hearts[i]->setAnimationSpeed(2, 8);
		hearts[i]->addKeyframe(2, glm::vec2(0.66666666666f, 0.0f));

		hearts[i]->setAnimationSpeed(3, 8);
		hearts[i]->addKeyframe(3, glm::vec2(0.66666666666f, 0.33333333333f));

        hearts[i]->changeAnimation(0);
	}

	spritesheetItem.loadFromFile("images/item_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetItem.setMinFilter(GL_NEAREST);
	spritesheetItem.setMagFilter(GL_NEAREST);

	helmet = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.333333333333, 0.333333333333), &spritesheetItem, &shaderProgram);
	helmet->setNumberAnimations(1);
	helmet->setAnimationSpeed(0, 8);
	helmet->addKeyframe(0, glm::vec2(0.0f, 0.33333333333f));
	helmet->changeAnimation(0);
	helmet->setPosition(uiPos);

	rock = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.333333333333, 0.333333333333), &spritesheetItem, &shaderProgram);
	rock->setNumberAnimations(1);
	rock->setAnimationSpeed(0, 8);
	rock->addKeyframe(0, glm::vec2(0.666666666666f, 0.33333333333f));
	rock->changeAnimation(0);
	rock->setPosition(uiPos);

	for (int i = 0; i < 3; ++i) {
		potion[i] = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.333333333333, 0.333333333333), &spritesheetItem, &shaderProgram);
		potion[i]->setNumberAnimations(1);
		potion[i]->setAnimationSpeed(0, 8);
		potion[i]->addKeyframe(0, glm::vec2(0.33333333333f, 0.666666666666f));
		potion[i]->changeAnimation(0);
		potion[i]->setPosition(uiPos);
	}

	for (int i = 0; i < 11; ++i) {
		bossHearts[i] = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.125, 0.125), &spritesheetBoss, &shaderProgram);
		bossHearts[i]->setNumberAnimations(4);

		bossHearts[i]->setAnimationSpeed(0, 8);
		bossHearts[i]->addKeyframe(0, glm::vec2(0.0f, 0.0f));

		bossHearts[i]->setAnimationSpeed(1, 8);
		bossHearts[i]->addKeyframe(1, glm::vec2(0.125f, 0.0f));

		bossHearts[i]->setAnimationSpeed(2, 8);
		bossHearts[i]->addKeyframe(2, glm::vec2(0.25f, 0.0f));

		bossHearts[i]->setAnimationSpeed(3, 8);
		bossHearts[i]->addKeyframe(3, glm::vec2(0.375f, 0.0f));

		bossHearts[i]->changeAnimation(0);
	}
}

void UI::update(int deltaTime, const glm::vec2& cameraPos, bool spear)
{
	num_hearts = playerMaxHealth / 3;
	full_hearts = playerHealth / 3;
	last_heart = playerHealth % 3;

	full_heartsBoss = bossHealth / 3;
	last_heartBoss = bossHealth % 3;

    if (spear) {
		spearActive = true;
        this->spear->setPosition(glm::vec2(cameraPos.x + 10, cameraPos.y + 10));
        this->spear->update(deltaTime);
    }
    else {
		spearActive = false;
        fire->setPosition(glm::vec2(cameraPos.x + 10, cameraPos.y + 10));
        fire->update(deltaTime);
    }

	for (int i = 0; i < num_hearts; ++i) {
		if (i < 6) hearts[i]->setPosition(glm::vec2(cameraPos.x + 10, cameraPos.y + 21 + 8 * (i + 1)));
		else hearts[i]->setPosition(glm::vec2(cameraPos.x + 10 + 8, cameraPos.y + 21 + 8 * (i - 6 + 1)));

		if (i < full_hearts) hearts[i]->changeAnimation(0);
		else if (i == full_hearts) {
			if (last_heart == 1) hearts[i]->changeAnimation(2);
			else if (last_heart == 2) hearts[i]->changeAnimation(1);
			else hearts[i]->changeAnimation(3);
		}
		else hearts[i]->changeAnimation(3);

		hearts[i]->update(deltaTime);
	}
	if (bossfight) {
		for (int i = 0; i < 11; ++i) {
			if (i < 6) bossHearts[i]->setPosition(glm::vec2(cameraPos.x + 26, cameraPos.y + 21 + 8 * (i + 1)));
			else bossHearts[i]->setPosition(glm::vec2(cameraPos.x + 26 + 8, cameraPos.y + 21 + 8 * (i - 6 + 1)));
			bossHearts[i]->update(deltaTime);

			if (i < full_heartsBoss) bossHearts[i]->changeAnimation(0);
			else if (i == full_heartsBoss) {
				if (last_heartBoss == 1) bossHearts[i]->changeAnimation(2);
				else if (last_heartBoss == 2) bossHearts[i]->changeAnimation(1);
				else bossHearts[i]->changeAnimation(3);
			}
			else bossHearts[i]->changeAnimation(3);
		}
	}

	if (playerAttackingHits > 0) {
		rock->setPosition(glm::vec2(cameraPos.x + 10, cameraPos.y + 208));
		rock->update(deltaTime);
	}
	else {
		rock->setPosition(glm::vec2(-100, -100));
		rock->update(deltaTime);
	}

	if (playerDefensiveHits > 0) {
		helmet->setPosition(glm::vec2(cameraPos.x + 34, cameraPos.y + 208));
		helmet->update(deltaTime);
	}
	else {
		helmet->setPosition(glm::vec2(-100, -100));
		helmet->update(deltaTime);
	}

	for (int i = 0; i < playerPotions; ++i) {
		potion[i]->setPosition(glm::vec2(cameraPos.x + 26 + 10 * i, cameraPos.y + 10));
		potion[i]->update(deltaTime);
	}
}

void UI::render()
{
    if (spearActive) spear->render();
	else fire->render();
	for (int i = 0; i < num_hearts; ++i) {
		hearts[i]->render();
	}
	helmet->render();
	rock->render();
	for (int i = 0; i < playerPotions; ++i) {
		potion[i]->render();
	}
	if (bossfight) {
		for (int i = 0; i < 11; ++i) {
			bossHearts[i]->render();
		}
	}
}

void UI::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void UI::setPosition(const glm::vec2& pos)
{
    posUI = pos;
    if (spearActive) spear->setPosition(posUI);
	else fire->setPosition(posUI);
}

void UI::setPlayerHealth(int health)
{
	playerHealth = health;
}

void UI::setPlayerMaxHealth(int health)
{
	playerMaxHealth = health;
}

void UI::setPlayerDefensiveHits(int hits)
{
	playerDefensiveHits = hits;
}

void UI::setPlayerAttackingHits(int hits)
{
	playerAttackingHits = hits;
}

void UI::setBossfight()
{
	bossfight = true;
}

void UI::setBossHealth(int health)
{
	bossHealth = health;
}

void UI::setPlayerPotions(int potions)
{
	playerPotions = potions;
}