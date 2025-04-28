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

	texture.setRepeated(true);
	texture.setSmooth(false);

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

	// Create a separate vertex array for each texture
	std::map<int, sf::VertexArray> texturedWalls;

	// Initialize vertex arrays for each texture
	for (const auto& pair : wallTextures) {
		texturedWalls[pair.first] = sf::VertexArray(sf::PrimitiveType::Triangles);
	}

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

		// Calculate raw perpendicular distance and apply fish-eye correction
		float rawDist;
		if (side == 0)
			rawDist = (sideDistX - deltaDistX);
		else
			rawDist = (sideDistY - deltaDistY);
		perpWallDist = rawDist * cos(playerAngle - rayAngle);

		// Determine line height on screen
		int lineHeight = static_cast<int>(screenHeight / perpWallDist);
		int drawStart = std::max(0, screenHeight / 2 - lineHeight / 2);
		int drawEnd = std::min(screenHeight - 1, screenHeight / 2 + lineHeight / 2);

		// UV mapping: calculate exact hit position on wall
		float wallX;
		if (side == 0)
			wallX = playerY / tileSize + rawDist * rayDirY;
		else
			wallX = playerX / tileSize + rawDist * rayDirX;
		wallX -= std::floor(wallX);

		int texX = int(wallX * textureSize);
		if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
			texX = textureSize - texX - 1;

		// Select texture
		int texId = wallTextures.count(wallType) ? wallType : 1;
		const sf::Texture& tex = wallTextures[texId];
		sf::Vector2f texSize = static_cast<sf::Vector2f>(tex.getSize());

		// Calculate texture coordinates
		float u0 = float(texX);
		float u1 = float(texX + 1);

		// Apply slight darkening to side walls for better depth perception
		sf::Color wallColor = (side == 1) ? sf::Color(180, 180, 180) : sf::Color::White;

		// Get the vertex array for this texture
		sf::VertexArray& currentWalls = texturedWalls[texId];

		// Triangle 1: top-left, top-right, bottom-right
		currentWalls.append(sf::Vertex{ { float(ray), float(drawStart) }, wallColor, { u0, 0.f } });
		currentWalls.append(sf::Vertex{ { float(ray + 1), float(drawStart) }, wallColor, { u1, 0.f } });
		currentWalls.append(sf::Vertex{ { float(ray + 1), float(drawEnd) }, wallColor, { u1, textureSize } });

		// Triangle 2: bottom-right, bottom-left, top-left
		currentWalls.append(sf::Vertex{ { float(ray + 1), float(drawEnd) }, wallColor, { u1, textureSize } });
		currentWalls.append(sf::Vertex{ { float(ray), float(drawEnd) }, wallColor, { u0, textureSize } });
		currentWalls.append(sf::Vertex{ { float(ray), float(drawStart) }, wallColor, { u0, 0.f } });

	}

	// Draw each textured wall group separately
	for (const auto& pair : texturedWalls) {
		// Skip empty vertex arrays
		if (pair.second.getVertexCount() == 0)
			continue;

		sf::RenderStates states;
		states.texture = &wallTextures[pair.first];
		target.draw(pair.second, states);
	}
}
