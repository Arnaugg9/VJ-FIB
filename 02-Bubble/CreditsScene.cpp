#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "CreditsScene.h"
#include "Game.h"

CreditsScene::CreditsScene()
{
}

CreditsScene::~CreditsScene()
{
	glDeleteTextures(1, &textureColorbuffer);
	glDeleteRenderbuffers(1, &rbo);
}

void CreditsScene::init()
{
	medio = false;
	fin = false;
	posTransition = 225;
	timeTransition = 200;
	finmedio = false;
	renderRestart = 0;

	initShaders();
	currentTime = 0.0f;
	projection = glm::ortho(0.0f, float(256), float(224), 0.0f);

	spritesheet.loadFromFile("images/spritesheet_credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	fondo = Sprite::createSprite(glm::ivec2(256, 224), glm::vec2(0.3333333333333f, 0.29f), &spritesheet, &texProgram);
	fondo->setNumberAnimations(1);
	fondo->setAnimationSpeed(0, 8);
	fondo->addKeyframe(0, glm::vec2(0.0f, 0.0f));
	fondo->changeAnimation(0);

	nombres = Sprite::createSprite(glm::ivec2(256, 224), glm::vec2(0.3333333333333f, 0.3333333333333f), &spritesheet, &texProgram);
	nombres->setNumberAnimations(1);
	nombres->setAnimationSpeed(0, 8);
	nombres->addKeyframe(0, glm::vec2(0.3333333333333f, 0.0f));
	nombres->changeAnimation(0);
	nombres->setPosition(glm::vec2(5, posTransition));
	//53

	indio1 = Sprite::createSprite(glm::ivec2(256, 224), glm::vec2(0.3333333333333, 0.3333333333333), &spritesheet, &texProgram);
	indio1->setNumberAnimations(1);
	indio1->setAnimationSpeed(0, 2);
	indio1->addKeyframe(0, glm::vec2(0.0f, 0.3333333333333f));
	indio1->addKeyframe(0, glm::vec2(0.333333333f, 0.3333333333333f));
	indio1->addKeyframe(0, glm::vec2(0.666666666f, 0.3333333333333f));
	indio1->addKeyframe(0, glm::vec2(0.333333333f, 0.3333333333333f));
	indio1->changeAnimation(0);
	indio1->setPosition(glm::vec2(82, posTransition + 67));
	//120

	indio2 = Sprite::createSprite(glm::ivec2(256, 224), glm::vec2(0.3333333333333, 0.3333333333333), &spritesheet, &texProgram);
	indio2->setNumberAnimations(1);
	indio2->setAnimationSpeed(0, 2);
	indio2->addKeyframe(0, glm::vec2(0.0f, 0.666666666f));
	indio2->addKeyframe(0, glm::vec2(0.333333333f, 0.666666666f));
	indio2->addKeyframe(0, glm::vec2(0.666666666f, 0.666666666f));
	indio2->addKeyframe(0, glm::vec2(0.333333333f, 0.666666666f));
	indio2->changeAnimation(0);
	indio2->setPosition(glm::vec2(146, posTransition + 67));
	//120

	restart = Sprite::createSprite(glm::ivec2(256, 224), glm::vec2(0.3333333333333, 0.3333333333333), &spritesheet, &texProgram);
	restart->setNumberAnimations(1);
	restart->setAnimationSpeed(0, 8);
	restart->addKeyframe(0, glm::vec2(0.666666666f, 0.0f));
	restart->changeAnimation(0);
	restart->setPosition(glm::vec2(0, posTransition + 291));

	SoundManager::playMusic("sounds/music/15_Credits.wav", true);
}

void CreditsScene::update(int deltaTime)
{
	currentTime += deltaTime;
	fondo->update(deltaTime);

	if (posTransition == 53) {
		medio = true;
	}

	if (medio && !finmedio) {
		--timeTransition;
		indio1->update(deltaTime);
		indio2->update(deltaTime);
		if (timeTransition < 0) {
			finmedio = true;
			medio = false;
		}
	}

	if (!medio && posTransition != -190) {
		--posTransition;
		nombres->setPosition(glm::vec2(5, posTransition));
		indio1->setPosition(glm::vec2(82, posTransition + 67));
		indio2->setPosition(glm::vec2(146, posTransition + 67));
		restart->setPosition(glm::vec2(0, posTransition + 291));
	}

	if (posTransition == -190) fin = true;


	nombres->update(deltaTime);
}

void CreditsScene::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);

	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);

	++renderRestart;

	fondo->render();
	nombres->render();
	indio1->render();
	indio2->render();
	if (!fin) restart->render();
	if (renderRestart % 64 < 32) restart->render();
}

void CreditsScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << std::endl;
		std::cout << "" << vShader.log() << std::endl << std::endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << std::endl;
		std::cout << "" << fShader.log() << std::endl << std::endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << std::endl;
		std::cout << "" << texProgram.log() << std::endl << std::endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void CreditsScene::handleKeyPress(int key)
{
	if (fin) {
		SoundManager::playSFX("sounds/effects/pause.wav");
		Game::instance().init();
	}
}
