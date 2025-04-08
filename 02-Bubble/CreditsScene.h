#ifndef _CREDITSSCENE_INCLUDE
#define _CREDITSSCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"

class CreditsScene
{
public:
    CreditsScene();
    ~CreditsScene();

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

    int timeTransition;
    int posTransition;
    bool medio;
    bool finmedio;
    bool fin;

    Texture spritesheet;
    Sprite* fondo;
    Sprite* nombres;
    Sprite* restart;
    int renderRestart;
    Sprite* indio1;
    Sprite* indio2;

    GLuint textureColorbuffer;
    GLuint rbo;
};

#endif // _CREDITSSCENE_INCLUDE
