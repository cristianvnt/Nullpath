#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <SFML/Graphics.hpp>
#include <Core/Constants.h>
#include <vector>
#include <map>

struct RayHit
{
	float hitWorldX;
	float hitWorldY;
	float distance;
};

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

	RayHit CastRay(float playerX, float playerY, float playerAngle);

	void Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle);
	void LoadTexture(int wallType, const std::string& texturePath);
};

#endif
