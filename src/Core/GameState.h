#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "State.h"
#include "Raycaster.h"
#include "Entities/Player.h"
#include "Minimap.h"
#include "Map.h"
#include "generators/DFSGenerator.h"

#include <vector>
#include <random>

class GameState : public State
{
private:
	Player* player;
	Raycaster* raycaster;
	Minimap* minimap;
	Map map;

	void InitWorld();

public:
	GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states);
	virtual ~GameState();

	void HandleKey(sf::Keyboard::Key code) override;
	void Update(float dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
	int GetCell(int x, int y) const;
};

#endif