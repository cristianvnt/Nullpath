#include "Raycaster.h"

void Raycaster::LoadTextures()
{
	LoadTexture(1, "Resources/Textures/wall3.png");
}

void Raycaster::LoadTexture(int wallType, const std::string& texturePath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(texturePath))
		std::cerr << "Failed to load texture: " << texturePath << "\n";

	texture.setSmooth(true);

	wallTextures[wallType] = std::move(texture);
}

Raycaster::Raycaster(int screenWidth, int screenHeight, const int* mapData, int mapWidth, int mapHeight, int cellSize)
	: screenWidth(screenWidth), screenHeight(screenHeight), 
	mapData(mapData), mapWidth(mapWidth), mapHeight(mapHeight), cellSize(cellSize)
{
	LoadTextures();
}

RayHit Raycaster::CastRay(float playerX, float playerY, float rayAngle)
{
	float rayDirX = cos(rayAngle);
	float rayDirY = sin(rayAngle);

	// Current map cell of player
	int mapX = static_cast<int>(playerX / cellSize);
	int mapY = static_cast<int>(playerY / cellSize);

	// Distance to next x or y-side
	float deltaDistX = (rayDirX == 0.f) ? 1e30f : std::abs(1.0f / rayDirX);
	float deltaDistY = (rayDirY == 0.f) ? 1e30f : std::abs(1.0f / rayDirY);

	float sideDistX, sideDistY;
	int stepX, stepY;

	// Calculate step and initial sideDist
	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = ((playerX / cellSize) - mapX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (mapX + 1.0f - (playerX / cellSize)) * deltaDistX;
	}

	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = ((playerY / cellSize) - mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (mapY + 1.0f - (playerY / cellSize)) * deltaDistY;
	}

	// DDA loop
	bool hit = false;
	int side = 0;

	while (!hit)
	{
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

		if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight)
			break;
		else if (mapData[mapY * mapWidth + mapX] > 0)
			hit = true;
	}

	// Calculate raw perpendicular distance
	float rawDist;
	if (side == 0)
		rawDist = (sideDistX - deltaDistX);
	else
		rawDist = (sideDistY - deltaDistY);

	float perpDistCells = rawDist;

	float perpDistPixels = perpDistCells * cellSize;
	float hitX = playerX + rayDirX * perpDistPixels;
	float hitY = playerY + rayDirY * perpDistPixels;

	return RayHit{ hitX, hitY, perpDistPixels };
}

void Raycaster::Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle)
{
	const int numRays = screenWidth;
	const float fov = Math::PI / 3.f; // 60 degrees field of view
	const float rayAngleStep = fov / static_cast<float>(numRays);

	// Floor and ceiling
	sf::RectangleShape ceiling({static_cast<float>(screenWidth), static_cast<float>(screenHeight) / 2.f});
	ceiling.setFillColor(sf::Color(102, 178, 255));
	target.draw(ceiling);

	sf::RectangleShape floor({static_cast<float>(screenWidth), static_cast<float>(screenHeight) / 2.f});
	floor.setFillColor(sf::Color(76, 153, 0));
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
		int mapX = static_cast<int>(playerX / cellSize);
		int mapY = static_cast<int>(playerY / cellSize);

		// Distance to next x or y-side
		float deltaDistX = (rayDirX == 0.f) ? 1e30f : std::abs(1.0f / rayDirX);
		float deltaDistY = (rayDirY == 0.f) ? 1e30f : std::abs(1.0f / rayDirY);

		float sideDistX, sideDistY;
		int stepX, stepY;

		// Calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = ((playerX / cellSize) - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - (playerX / cellSize)) * deltaDistX;
		}

		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = ((playerY / cellSize) - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - (playerY / cellSize)) * deltaDistY;
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

		// Calculate original (unclamped) draw positions
		float originalDrawStart = screenHeight / 2.f - lineHeight / 2.f;
		float originalDrawEnd = screenHeight / 2.f + lineHeight / 2.f;

		// Apply clipping for rendering
		float clampedDrawStart = originalDrawStart;
		float clampedDrawEnd = originalDrawEnd;

		if (clampedDrawStart < 0.f)
			clampedDrawStart = 0.f;
		if (clampedDrawEnd >= screenHeight)
			clampedDrawEnd = screenHeight - 1.f;

		// UV mapping: calculate exact hit position on wall
		float wallX;
		if (side == 0)
			wallX = playerY / cellSize + rawDist * rayDirY;
		else
			wallX = playerX / cellSize + rawDist * rayDirX;
		wallX -= std::floor(wallX);

		int texX = int(wallX * textureSize);
		if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
			texX = textureSize - texX - 1;

		// Select texture
		int texId = wallTextures.count(wallType) ? wallType : 1;
		const sf::Texture& tex = wallTextures[texId];
		sf::Vector2f texSize = static_cast<sf::Vector2f>(tex.getSize());

		// Calculate texture coordinates based on visible portion
		float totalWallHeight = originalDrawEnd - originalDrawStart;
		float visibleTop = (clampedDrawStart - originalDrawStart) / totalWallHeight;
		float visibleBottom = (clampedDrawEnd - originalDrawStart) / totalWallHeight;

		float u0 = float(texX);
		float u1 = float(texX + 1);
		float v0 = visibleTop * textureSize;
		float v1 = visibleBottom * textureSize;

		// Apply slight darkening to side walls for better depth perception
		sf::Color wallColor = (side == 1) ? sf::Color(180, 180, 180) : sf::Color::White;

		// Get the vertex array for this texture
		sf::VertexArray& currentWalls = texturedWalls[texId];

		// Triangle 1: top-left, top-right, bottom-right
		currentWalls.append(sf::Vertex{ { float(ray), clampedDrawStart }, wallColor, { u0, v0 } });
		currentWalls.append(sf::Vertex{ { float(ray + 1), clampedDrawStart }, wallColor, { u1, v0 } });
		currentWalls.append(sf::Vertex{ { float(ray + 1), clampedDrawEnd }, wallColor, { u1, v1 } });

		// Triangle 2: bottom-right, bottom-left, top-left
		currentWalls.append(sf::Vertex{ { float(ray + 1), clampedDrawEnd }, wallColor, { u1, v1 } });
		currentWalls.append(sf::Vertex{ { float(ray), clampedDrawEnd }, wallColor, { u0, v1 } });
		currentWalls.append(sf::Vertex{ { float(ray), clampedDrawStart }, wallColor, { u0, v0 } });

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
