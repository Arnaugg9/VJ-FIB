#ifndef _MENUSCENE_INCLUDE
#define _MENUSCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "SoundManager.h"

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

private:
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;

    Texture spritesheet;
	Sprite* sprite;
    float animationTime;

    bool startGame;
    bool transitioning;
    float transitionAlpha;
	bool initTransition;
    bool sound;

    GLuint textureColorbuffer;
    GLuint rbo;
};

#endif // _MENUSCENE_INCLUDE
