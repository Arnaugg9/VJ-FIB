#ifndef ENEMY_H
#define ENEMY_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Sprite.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Bullet.h"


class Enemy
{
public:
    virtual ~Enemy() = default; // Destructor virtual para asegurar eliminación correcta en herencia

    virtual void init(const glm::ivec2& enemyPos, ShaderProgram& shaderProgram);
    virtual void update(int deltaTime);
    virtual void render();

    void setPosition(const glm::vec2& pos);
    glm::ivec2 getPosition() const;
    glm::ivec2 getSize() const;
    glm::ivec2 getColliderPosition() const;
    glm::ivec2 getColliderSize() const;

    bool isOnScreen() const;
    void setOnScreen(bool state);

    bool getHasBullet();

    void getHurt(int damage);

    int getDamage();

    void setMap(TileMap* tileMap);

    virtual Bullet* getBullet();

protected:
    //Data enemy position / collider
    glm::ivec2 posEnemy, sizeEnemy;
    glm::ivec2 posCollider, sizeCollider;

    //Data enemy properties
    int damage;
    int health;

    bool hasBullet;

    //Controlen direccio
    int timeChangeDirection;
    int direction;

    bool onScreen;

    //sprite data
    Sprite* sprite;
    Texture spritesheet;

    //Pels enemies que el necessiten 
    glm::ivec2 tileMapDispl;
    TileMap* map;

};

#endif // ENEMY_H

