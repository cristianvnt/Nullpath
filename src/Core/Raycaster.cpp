#include "Raycaster.h"

void Raycaster::LoadTextures()
{
	LoadTexture(1, "Resources/Textures/brickwall.png");
	LoadTexture(2, "Resources/Textures/stonewall.png");
	LoadTexture(3, "Resources/Textures/woodwall.png");
}

void Raycaster::LoadTexture(int wallType, const std::string& texturePath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(texturePath))
		std::cerr << "Failed to load texture: " << texturePath << "\n";
	wallTextures[wallType] = std::move(texture);
}

Raycaster::Raycaster(int screenWidth, int screenHeight, const int* mapData, int mapWidth, int mapHeight, int tileSize)
	: screenWidth(screenWidth), screenHeight(screenHeight), 
	mapData(mapData), mapWidth(mapWidth), mapHeight(mapHeight), tileSize(tileSize)
{
	LoadTextures();
}

void Raycaster::Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle)
{
	const int numRays = screenWidth;
	const float fov = Math::PI / 3.f; // 60 degrees field of view
	const float rayAngleStep = fov / static_cast<float>(numRays);

	// Floor and ceiling
	sf::RectangleShape ceiling({static_cast<float>(screenWidth), static_cast<float>(screenHeight) / 2.f});
	ceiling.setFillColor(sf::Color(100, 100, 100));
	target.draw(ceiling);

	sf::RectangleShape floor({static_cast<float>(screenWidth), static_cast<float>(screenHeight) / 2.f});
	floor.setFillColor(sf::Color(50, 50, 50));
	floor.setPosition({ 0.f, static_cast<float>(screenHeight) / 2.f });
	target.draw(floor);

	for (int ray = 0; ray < numRays; ray++)
	{
		// Compute ray angle relative to player direction
		float rayAngle = playerAngle - fov / 2.f + ray * rayAngleStep;

		// Normalize the angle
		rayAngle = std::fmod(rayAngle + 2 * Math::PI, 2 * Math::PI);

		float rayDirX = cos(rayAngle);
		float rayDirY = sin(rayAngle);

		// Current map cell of player
		int mapX = static_cast<int>(playerX / tileSize);
		int mapY = static_cast<int>(playerY / tileSize);

		// Distance to next x or y-side
		float deltaDistX = (rayDirX == 0.f) ? 1e30f : std::abs(1.0f / rayDirX);
		float deltaDistY = (rayDirY == 0.f) ? 1e30f : std::abs(1.0f / rayDirY);

		float sideDistX, sideDistY;
		int stepX, stepY;

		// Calculate step and initial sideDist
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

		// DDA: perform grid traversal to find wall hit
		bool hit = false;
		int side = 0; // NS or a EW wall hit
		float perpWallDist;
		int wallType = 0;

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

			// Check for out-of-bound or wall hit
			if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight)
			{
				// Ray is out of bounds
				perpWallDist = screenWidth; // Set maximum distance
				hit = true;
			}
			else if (mapData[mapY * mapWidth + mapX] > 0)
			{
				wallType = mapData[mapY * mapWidth + mapX];
				hit = true;
			}
		}

		// Calculate perpendicular distance and apply fish-eye correction
		if (side == 0)
			perpWallDist = (sideDistX - deltaDistX);
		else
			perpWallDist = (sideDistY - deltaDistY);
		perpWallDist *= cos(playerAngle - rayAngle);

		// Determine line height on screen
		int lineHeight = static_cast<int>(screenHeight / perpWallDist);
		int drawStart = std::max(0, screenHeight / 2 - lineHeight / 2);
		int drawEnd = std::max(screenHeight - 1, screenHeight / 2 + lineHeight / 2);

		// UV mapping: calculate exact hit position on wall
		float wallX;
		if (side == 0)
			wallX = playerY / tileSize + perpWallDist * rayDirY;
		else
			wallX = playerX / tileSize + perpWallDist * rayDirX;
		wallX -= std::floor(wallX);

		int texX = int(wallX * textureSize);
		if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
			texX = textureSize - texX - 1;

		// Draw texture
		sf::RectangleShape slice(sf::Vector2f(1.f, static_cast<float>(drawEnd - drawStart)));
		slice.setPosition(sf::Vector2f(static_cast<float>(ray), static_cast<float>(drawStart)));

		int texId = wallTextures.count(wallType) ? wallType : 1;
		slice.setTexture(&wallTextures[texId]);
		slice.setTextureRect(sf::IntRect(sf::Vector2i(texX, 0), sf::Vector2i(1, textureSize)));
		target.draw(slice);
	}
}
