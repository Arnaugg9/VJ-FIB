#ifndef _MENUSCENE_INCLUDE
#define _MENUSCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"

class MenuScene
{
public:
    MenuScene();
    ~MenuScene();

    void init();
    void update(int deltaTime);
    void render();
    void handleKeyPress(int key);

private:
    void initShaders();
    void initFramebuffer();

private:
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    bool startGame;
    bool transitioning;
    float transitionAlpha;

    GLuint framebuffer;
    GLuint textureColorbuffer;
    GLuint rbo;
};

#endif // _MENUSCENE_INCLUDE
