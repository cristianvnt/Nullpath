#include "Raycaster.h"

Raycaster::Raycaster(int screenWidth, int screenHeight, const int* mapData, int mapWidth, int mapHeight, int tileSize)
	: screenWidth(screenWidth), screenHeight(screenHeight), 
	mapData(mapData), mapWidth(mapWidth), mapHeight(mapHeight), tileSize(tileSize)
{
}

void Raycaster::Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle)
{
	const int numRays = screenWidth;
	const float fov = Math::PI / 3.f; // 60 degrees field of view
	const float rayAngleStep = fov / static_cast<float>(numRays);

	// Floor and ceiling
	sf::RectangleShape ceiling(sf::Vector2f(screenWidth, screenHeight / 2));
	ceiling.setFillColor(sf::Color(100, 100, 100)); // Dark gray ceiling
	ceiling.setPosition(sf::Vector2f(0, 0));
	target.draw(ceiling);

	sf::RectangleShape floor(sf::Vector2f(screenWidth, screenHeight / 2));
	floor.setFillColor(sf::Color(50, 50, 50)); // Darker gray floor
	floor.setPosition(sf::Vector2f(0, screenHeight / 2));
	target.draw(floor);

	for (int ray = 0; ray < numRays; ray++)
	{
		float rayAngle = playerAngle - fov / 2.f + ray * rayAngleStep;

		// Normalize the angle
		while (rayAngle < 0) 
			rayAngle += 2 * Math::PI;
		while (rayAngle >= 2 * Math::PI)
			rayAngle -= 2 * Math::PI;

		float rayDirX = cos(rayAngle);
		float rayDirY = sin(rayAngle);

		// Current map position
		int mapX = static_cast<int>(playerX / tileSize);
		int mapY = static_cast<int>(playerY / tileSize);

		// Length of ray from current position to next x or y-side
		float sideDistX;
		float sideDistY;

		// Length of ray from one x or y-side to next x or y-side
		float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(1.0f / rayDirX);
		float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(1.0f / rayDirY);

		// What direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		// Calc step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = ((playerX / tileSize) - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - (playerX / tileSize)) * deltaDistX;
		}

		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = ((playerY / tileSize) - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - (playerY / tileSize)) * deltaDistY;
		}

		// DDA
		bool hit = false;
		int side = 0; // NS or a EW wall hit?
		float perpWallDist;

		while (!hit)
		{
			// Jump to next map square, either in x-direction, or in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}

			// Check if ray hit a wall
			if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight)
			{
				// Ray is out of bounds
				perpWallDist = screenWidth; // Set maximum distance
				hit = true;
			}
			else if (mapData[mapY * mapWidth + mapX] > 0)
			{
				hit = true;
			}
		}

		// Calc distance projected on camera direction
		if (side == 0)
			perpWallDist = (sideDistX - deltaDistX);
		else
			perpWallDist = (sideDistY - deltaDistY);

		// Adjust for fisheye effect
		perpWallDist *= cos(playerAngle - rayAngle);

		int lineHeight = static_cast<int>(screenHeight / perpWallDist);


		// Calc lowest and highest pixel to fill in current stripe
		int drawStart = screenHeight / 2 - lineHeight / 2;
		if (drawStart < 0) 
			drawStart = 0;

		int drawEnd = screenHeight / 2 + lineHeight / 2;
		if (drawEnd >= screenHeight) 
			drawEnd = screenHeight - 1;

		// Wall color based on side (for shading effect)
		sf::Color wallColor = sf::Color::White;
		if (side == 1)
		{
			// Darker shade for y-side walls
			wallColor = sf::Color(200, 200, 200);
		}

		sf::Vertex line[2];
		line[0].position = sf::Vector2f(static_cast<float>(ray), static_cast<float>(drawStart));
		line[0].color = wallColor;
		line[1].position = sf::Vector2f(static_cast<float>(ray), static_cast<float>(drawEnd));
		line[1].color = wallColor;

		target.draw(line, 2, sf::PrimitiveType::Lines);
	}
}
