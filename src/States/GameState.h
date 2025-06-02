#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "State.h"
#include "Core/Raycaster.h"
#include "Entities/Player.h"
#include "Core/Minimap.h"
#include "Core/Map.h"
#include "Generators/DFSGenerator.h"
#include "Generators/BSPGenerator.h"

#include <vector>
#include <random>

class GameState : public State
{
private:
	Player* player;
	Raycaster* raycaster;
	Minimap* minimap;
	Map map;

	bool showMinimap;

	void InitWorldDFS();
	void InitWorldBSP();

	void RegenerateWorld(const std::function<void(Map&)>& generator);

	// BSP debug state
	std::unique_ptr<BSPGenerator> bspGen;
	std::vector<std::pair<sf::Vector2f, int>> roomMarkers;
	bool showDebug = false;
	bool usingBSP = false;
	sf::Font& debugFont;

public:
	GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states, sf::Font& sharedFont);
	virtual ~GameState();

	void HandleKey(sf::Keyboard::Key code) override;
	void Update(float dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
	int GetCell(int x, int y) const;
};

#endif