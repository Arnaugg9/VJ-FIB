#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Sprite.h"
#include "ShaderProgram.h"

class Bullet {
public:
    void init(const glm::ivec2 size, int dir, Sprite* bulletSprite, int speed, int timeToLive);
    void initBoss(const glm::ivec2 size, int dir, Sprite* bulletSprite, int speed, int timeToLive);
    void update(int deltaTime);
    void render();

    glm::ivec2 getPosition();
    glm::ivec2 getSize();
    glm::ivec2 getColliderPosition();
    glm::ivec2 getColliderSize();
    bool isActive();
    void activate(glm::ivec2& posBullet, int dir);
    void activateBoss(glm::ivec2& posBullet, int dir, int type);
    void deactivate();

private:
    glm::ivec2 pos, size;
    glm::ivec2 posCollider, sizeCollider;
    int speed;
    int direction; 
    Sprite* sprite;

    int timeToLive;
    int timeRest;
    bool active;
};

#endif