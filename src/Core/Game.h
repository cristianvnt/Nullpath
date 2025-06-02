#ifndef GAME_H
#define GAME_H

#include "States/MainMenuState.h"

class Game
{
private:
	sf::RenderWindow* window;
	std::vector<sf::VideoMode> videoModes;
	sf::ContextSettings windowSettings;
	bool fullscreen;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	std::map<std::string, sf::Keyboard::Key> keybinds;

	void InitVariables();
	void InitWindow();
	void InitStates();
	void InitKeybinds();

public:
	Game();
	virtual ~Game();

	// Functions
	void EndApplication();

	// Updates
	void UpdateDt();
	void UpdateEvents();
	void Update();

	// Render
	void Render();

	// Core
	void Run();
};

#endif