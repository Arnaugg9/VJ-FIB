#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "MenuScene.h"
#include "Game.h"

MenuScene::MenuScene() : startGame(false), transitioning(false), transitionAlpha(0.0f), initTransition(true)
{
}

MenuScene::~MenuScene()
{
    glDeleteTextures(1, &textureColorbuffer);
    glDeleteRenderbuffers(1, &rbo);
}

void MenuScene::init()
{
    initShaders();
    currentTime = 0.0f;
    projection = glm::ortho(0.0f, float(256), float(224), 0.0f);

    spritesheet.loadFromFile("images/spritesheet_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(256, 224), glm::vec2(0.2f, 1.0f), &spritesheet, &texProgram);
    sprite->setNumberAnimations(5);

    for (int i = 0; i < 5; ++i) {
        sprite->setAnimationSpeed(i, 8);
        sprite->addKeyframe(i, glm::vec2(i * 0.2f, 0.0f));
    }
    sprite->changeAnimation(0);
}

void MenuScene::update(int deltaTime)
{
    currentTime += deltaTime;
    sprite->update(deltaTime);
    --animationTime;
    
	if (animationTime <= 0) {
        if (sprite->animation() == 0) sprite->changeAnimation(1);
        else if (sprite->animation() == 1) sprite->changeAnimation(2);
        else if (sprite->animation() == 2) sprite->changeAnimation(3);
        else if (sprite->animation() == 3) {
            sprite->changeAnimation(4);
			initTransition = false;
        }
		animationTime = 20;
	}

    if (startGame && !transitioning && !initTransition) {
        transitioning = true;
        transitionAlpha = 0.0f;
    }

    if (transitioning) {
        transitionAlpha += deltaTime * 0.0005f; // Ajustar velocidad

        if (transitionAlpha >= 1.0f) {
            Game::instance().changeScene(1);
        }
    }
}

void MenuScene::render()
{
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) * (1 - transitionAlpha);

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", color.r, color.g, color.b, color.a);

    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);

    sprite->render();
}

void MenuScene::handleKeyPress(int key)
{
    if (!initTransition) startGame = true;
}

void MenuScene::initShaders()
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