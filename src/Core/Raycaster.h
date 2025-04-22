#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <Core/Constants.h>

class Raycaster
{
private:
	int screenWidth;
	int screenHeight;
	int mapWidth;
	int mapHeight;
	int tileSize;

	const int* mapData;

public:
	Raycaster(int screenWidth, int screenHeight, const int* mapData, int mapWidth, int mapHeight, int tileSize);

	void Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle);
};

#endif
