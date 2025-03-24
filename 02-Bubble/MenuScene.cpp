#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "MenuScene.h"
#include "Game.h"

MenuScene::MenuScene() : startGame(false), transitioning(false), transitionAlpha(0.0f)
{
}

MenuScene::~MenuScene()
{
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &textureColorbuffer);
    glDeleteRenderbuffers(1, &rbo);
}

void MenuScene::init()
{
    initShaders();
    initFramebuffer();
    currentTime = 0.0f;
    projection = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue background
}

void MenuScene::initFramebuffer()
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MenuScene::update(int deltaTime)
{
    currentTime += deltaTime;

    if (startGame && !transitioning) {
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
    // **Dibujar la escena normal en el framebuffer**
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    texProgram.setUniform1f("uAlpha", 0.0f);  // No aplicar fade-out aún

    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // **Dibujar la textura del framebuffer en la pantalla**
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(SCREEN_WIDTH, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, SCREEN_HEIGHT);
    glEnd();

    // **Dibujar el fade-out negro si está en transición**
    if (transitioning) {
        texProgram.use();
        texProgram.setUniform1f("uAlpha", transitionAlpha); // Pasar alpha al shader

        glBindTexture(GL_TEXTURE_2D, 0);

        glBegin(GL_QUADS);
        glColor4f(0.0f, 0.0f, 0.0f, transitionAlpha);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(SCREEN_WIDTH, 0.0f);
        glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
        glVertex2f(0.0f, SCREEN_HEIGHT);
        glEnd();
    }

    glDisable(GL_BLEND);
}

void MenuScene::handleKeyPress(int key)
{
    startGame = true;
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