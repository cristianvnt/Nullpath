#ifndef BPSGENERATOR_H
#define BPSGENERATOR_H

#include <SFML/Graphics.hpp>
#include "Core/Map.h"
#include "Core/Minimap.h"
#include "BSPNode.h"

class BSPGenerator
{
private:
	int minRoomSizeCells;
	float splitRatio;
	int paddingCells;

	std::unique_ptr<BSPNode> rootNode;
	std::vector<std::pair<sf::Vector2f, int>> roomMarkers;

public:
	explicit BSPGenerator(int minRoomSizeCells = 3, float splitRatio = 1.25f, int paddingCells = 1);
	void Generate(Map& map);

	const std::vector<std::pair<sf::Vector2f, int>>& GetRoomMarkers() const { return roomMarkers; }

	// Expose the built BSP tree root for debug rendering
	const BSPNode* GetRootNode() const { return rootNode.get(); }

	// Draw split lines and room boxes
	void RenderDebug(sf::RenderTarget& target, const Map& map) const;
};

#endif // !BPSGENERATOR_H
