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

	void InitRaycaster();

	// DFS based maze generation methods
	void GenerateMazeDFS();
	void CarveDFS(int x, int y);

	void RenderMiniMap(sf::RenderTarget& target);

public:
	GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states);
	virtual ~GameState();

	void HandleKey(sf::Keyboard::Key code) override;

	void Update(float dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;

	int GetTile(int x, int y) const;
};

#endif