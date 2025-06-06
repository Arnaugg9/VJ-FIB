#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "GameScene.h"
#include "MenuScene.h"
#include "CreditsScene.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960


// Game is a singleton (a class with a single instance) that represents our whole application

class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();

	void changeScene(int scene);
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	GameScene gameScene;
	MenuScene menuScene;
	CreditsScene creditsScene;
	int currentScene;

};


#endif // _GAME_INCLUDE


