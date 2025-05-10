#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "State.h"
#include "Raycaster.h"
#include "Entities/Player.h"

#include <vector>
#include <random>

class GameState : public State
{
private:
	Player* player;
	Raycaster* raycaster;

	int mapWidth;
	int mapHeight;
	int tileSize;
	int* mapData;

	std::vector<bool> visited;
	// RNG engine for maze generation and random placements
	std::mt19937 rng;

	// DFS based maze generation methods
	void generateMazeDFS();
	void carveDFS(int x, int y);

	void InitRaycaster();

public:
	GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states);
	virtual ~GameState();

	void Update(float dt) override;
	void UpdateInput() override;
	void Render(sf::RenderTarget* target = nullptr) override;

	int GetTile(int x, int y) const;
};

#endif