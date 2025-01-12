#ifndef GAME_H
#define GAME_H

#include "../States/GameState.h"

class Game
{
private:
	sf::RenderWindow* window;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	void InitWindow();
	void InitStates();

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