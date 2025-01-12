#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"

class GameState : public State
{
private:
	Entity player;

	// Functions
	void InitKeybinds() override;

public:
	GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys);
	virtual ~GameState();

	void EndState() override;
	void UpdateInput(const float& dt) override;
	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
};

#endif