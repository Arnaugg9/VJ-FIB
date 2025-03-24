#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


enum MyEnum
{
	MENU_SCENE, GAME_SCENE
};

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	gameScene.init();
	menuScene.init();

	currentScene = MENU_SCENE;
}

bool Game::update(int deltaTime)
{
	if (currentScene == MENU_SCENE) menuScene.update(deltaTime);
	else if (currentScene == GAME_SCENE) gameScene.update(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (currentScene == MENU_SCENE)menuScene.render();
	else if (currentScene == GAME_SCENE) gameScene.render();
}

void Game::changeScene(int scene) {
	currentScene = scene;
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;

	/*
	if (key == GLFW_KEY_P)
		scene.getPlayer()->test();
	*/

	if (currentScene == MENU_SCENE) menuScene.handleKeyPress(key);


	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



