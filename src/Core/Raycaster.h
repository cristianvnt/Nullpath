#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <Core/Constants.h>
#include <map>

class Raycaster
{
private:
	int screenWidth;
	int screenHeight;
	int mapWidth;
	int mapHeight;
	int tileSize;

	const int* mapData;

	std::map<int, sf::Texture> wallTextures;
	static const int textureSize = 64;

	void LoadTextures();

public:
	Raycaster(int screenWidth, int screenHeight, const int* mapData, int mapWidth, int mapHeight, int tileSize);

	void Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle);
	void LoadTexture(int wallType, const std::string& texturePath);
};

#endif
