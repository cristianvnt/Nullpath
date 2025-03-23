#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "State.h"

class GameState : public State
{
private:
	Player* player;

	void InitPlayers();

public:
	GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states);
	virtual ~GameState();

	void Update(float dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
};

#endif