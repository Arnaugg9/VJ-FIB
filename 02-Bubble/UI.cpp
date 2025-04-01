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

    spritesheet.loadFromFile("images/UI_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    spear = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    spear->setNumberAnimations(1);
    spear->setAnimationSpeed(0, 8);
    spear->addKeyframe(0, glm::vec2(0.0f, 0.33333333333f));
    spear->changeAnimation(0);
    spear->setPosition(uiPos);

    fire = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
    fire->setNumberAnimations(1);
    fire->setAnimationSpeed(0, 8);
    fire->addKeyframe(0, glm::vec2(0.33333333333f, 0.33333333333f));
    fire->changeAnimation(0);
    fire->setPosition(uiPos);

	for (int i = 0; i < 12; ++i) {
		hearts[i] = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.33, 0.33), &spritesheet, &shaderProgram);
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
}

void UI::update(int deltaTime, const glm::vec2& cameraPos, bool spear)
{
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
	for (int i = 0; i < 12; ++i) {
		if (i < 6) hearts[i]->setPosition(glm::vec2(cameraPos.x + 10, cameraPos.y + 21 + 8 * (i + 1)));
		else hearts[i]->setPosition(glm::vec2(cameraPos.x + 10 + 8, cameraPos.y + 21 + 8 * (i - 6 + 1)));
		hearts[i]->update(deltaTime);
	}
}

void UI::render()
{
    if (spearActive) spear->render();
	else fire->render();
	for (int i = 0; i < 12; ++i) {
		hearts[i]->render();
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