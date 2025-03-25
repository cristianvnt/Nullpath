#include "Raycaster.h"

Raycaster::Raycaster(int screenWidth, int screenHeight, const int* mapData, int mapWidth, int mapHeight, int tileSize)
	: screenWidth(screenWidth), screenHeight(screenHeight), 
	mapData(mapData), mapWidth(mapWidth), mapHeight(mapHeight), tileSize(tileSize)
{
}

void Raycaster::Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle)
{
	const int numRays = screenWidth;
	const float fov = 3.14159f / 3.f; // 60 degrees field of view
	const float rayAngleStep = fov / static_cast<float>(numRays);

	for (int ray = 0; ray < numRays; ray++)
	{
		float rayAngle = playerAngle - fov / 2.f + ray * rayAngleStep;

		float rayX = cos(rayAngle);
		float rayY = sin(rayAngle);

		float distToWall = 0.f;
		const float maxDist = 1000.f;
		bool hitWall = false;

		while (!hitWall && distToWall < maxDist)
		{
			distToWall += 1.f;

			int testX = static_cast<int>((playerX + rayX * distToWall) / tileSize);
			int testY = static_cast<int>((playerY + rayY * distToWall) / tileSize);

			if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight)
			{
				hitWall = true;
				distToWall = maxDist;
			}
			else if (mapData[testY * mapWidth + testX] == 1)
				hitWall = true;
		}

		int lineHeight = static_cast<int>((screenHeight / distToWall) * tileSize);
		int drawStart = (screenHeight / 2) - (lineHeight / 2);
		int drawEnd = drawStart + lineHeight;

		sf::Vertex line[2];
		line[0].position = sf::Vector2f(static_cast<float>(ray), static_cast<float>(drawStart));
		line[0].color = sf::Color::White;
		line[1].position = sf::Vector2f(static_cast<float>(ray), static_cast<float>(drawEnd));
		line[1].color = sf::Color::White;

		target.draw(line, 2, sf::PrimitiveType::Lines);
	}
}
