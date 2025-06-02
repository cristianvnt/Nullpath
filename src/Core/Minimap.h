#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include "Raycaster.h"
#include "Generators/BSPNode.h"

class Minimap
{
private:
	int mapWidth;
	int mapHeight;
	int cellSize;
	const int* mapData;
	Raycaster* raycaster;

	float margin = 10.f;
	float scaleX = 1.f;
	float scaleY = 1.f;
	float maxWidth = 600.f;
	float maxHeight = 400.f;
	float fov = 60.f * (Math::PI / 180.f);
	int slices = 60;

	std::vector<std::pair<sf::Vector2f, int>> roomMarkers;
	sf::Font& font;
	const BSPNode* debugRoot = nullptr;

	bool drawRoomIDs = false;

public:
	Minimap(int mapWidth, int mapHeight, int cellSize, const int* mapData, Raycaster* raycaster, 
		const std::vector<std::pair<sf::Vector2f, int>>& markers, sf::Font& f, const BSPNode* debugRoot = nullptr);

	void SetDrawRoomIDs(bool b) { drawRoomIDs = b; }
	bool GetDrawRoomIDs() const { return drawRoomIDs; }

	// World size to minimap
	sf::FloatRect WorldToMini(const sf::FloatRect& worldRect) const;
	sf::Vector2f WorldToMiniPoint(const sf::Vector2f& worldPoint) const;

	void Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle);
};

#endif // !MINIMAP_H
