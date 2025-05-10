#include "Minimap.h"

Minimap::Minimap(int mapWidth, int mapHeight, int tileSize, const int* mapData, Raycaster* raycaster)
	: mapWidth(mapWidth), mapHeight(mapHeight), tileSize(tileSize), mapData(mapData), raycaster(raycaster)
{
}

void Minimap::Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle)
{
	float miniTile = tileSize * 0.6f;
	sf::RectangleShape cell({ miniTile, miniTile });
	const float margin = 10.f;
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			int v = mapData[y * mapWidth + x];
			cell.setFillColor(v > 0 ? sf::Color(30, 60, 60) : sf::Color(180, 180, 180));
			cell.setPosition({ margin + x * miniTile, margin + y * miniTile });
			target.draw(cell);
		}
	}

	// Draw border around
	sf::RectangleShape border({ mapWidth * miniTile + 2.f, mapHeight * miniTile + 2.f });
	border.setPosition({ margin - 1.f, margin - 1.f });
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineColor(sf::Color::White);
	border.setOutlineThickness(3.f);
	target.draw(border);

	// World player position
	float worldX = playerX;
	float worldY = playerY;

	// Grid coordinates
	float cellX = worldX / tileSize;
	float cellY = worldY / tileSize;

	// Mini-map dot coordinates
	float dotX = margin + cellX * miniTile;
	float dotY = margin + cellY * miniTile;

	// Dot player on mini-map
	sf::CircleShape dot(miniTile * 0.25f);
	dot.setOrigin({ dot.getRadius(), dot.getRadius() });
	dot.setFillColor(sf::Color::Red);
	dot.setPosition({ dotX, dotY });
	target.draw(dot);

	float angL = playerAngle - fov / 2;
	float angR = playerAngle + fov / 2;

	auto hitL = raycaster->CastRay(worldX, worldY, angL);
	auto hitR = raycaster->CastRay(worldX, worldY, angR);

	sf::Vector2f dotPlayer{ dotX, dotY };
	sf::Vector2f endPointL{
		margin + (hitL.hitWorldX / tileSize) * miniTile,
		margin + (hitL.hitWorldY / tileSize) * miniTile,
	};

	sf::Vector2f endPointR{
		margin + (hitR.hitWorldX / tileSize) * miniTile,
		margin + (hitR.hitWorldY / tileSize) * miniTile,
	};

	// Color the FOV
	const int slices = 60;
	sf::VertexArray fovFill(sf::PrimitiveType::TriangleFan, slices + 2);
	fovFill[0].position = dotPlayer;
	fovFill[0].color = sf::Color::Yellow;
	for (int i = 0; i <= slices; i++)
	{
		float rawA = playerAngle - fov / 2 + (fov / slices) * i;
		float a = std::fmod(rawA + 2 * Math::PI, 2 * Math::PI);

		// Cast ray at normalized angle
		auto hit = raycaster->CastRay(worldX, worldY, a);
		sf::Vector2f hitPoint{
			margin + (hit.hitWorldX / tileSize) * miniTile,
			margin + (hit.hitWorldY / tileSize) * miniTile,
		};

		fovFill[i + 1].position = hitPoint;
		fovFill[i + 1].color = sf::Color::Yellow;
	}

	target.draw(fovFill);
}
