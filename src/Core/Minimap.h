#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include "Raycaster.h"

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

public:
	Minimap(int mapWidth, int mapHeight, int cellSize, const int* mapData, Raycaster* raycaster);

	void Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle);
};

#endif // !MINIMAP_H
