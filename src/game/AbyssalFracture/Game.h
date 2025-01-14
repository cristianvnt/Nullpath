#ifndef GAME_H
#define GAME_H

#include "../States/MainMenuState.h"

class Game
{
private:
	sf::RenderWindow* window;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	std::map<std::string, sf::Keyboard::Key> supportedKeys;

	void InitWindow();
	void InitStates();
	void InitKeys();

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