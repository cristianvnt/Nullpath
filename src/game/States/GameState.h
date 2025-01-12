#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"

class GameState : public State
{
private:


public:
	GameState(sf::RenderWindow* window);
	virtual ~GameState();

	void EndState();
	void UpdateKeybinds(const float& dt) override;
	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
};

#endif