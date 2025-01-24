#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "States/Base/State.h"

class GameState : public State
{
private:
	Player* player;

	// Functions
	void InitKeybinds() override;
	void InitTextures();
	void InitPlayers();

public:
	GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states);
	virtual ~GameState();

	void UpdateInput(const float& dt) override;
	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
};

#endif