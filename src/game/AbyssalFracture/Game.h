#ifndef GAME_H
#define GAME_H

#include "../States/State.h"

class Game
{
private:
	sf::RenderWindow* window;

	sf::Clock dtClock;
	float dt;

	void InitWindow();

public:
	Game();
	virtual ~Game();

	void UpdateDt();
	void UpdateEvents();
	void Update();
	void Render();
	void Run();
};

#endif