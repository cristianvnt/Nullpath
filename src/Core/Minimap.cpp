#include "Minimap.h"

Minimap::Minimap(int mapWidth, int mapHeight, int cellSize, const int* mapData, Raycaster* raycaster)
	: mapWidth(mapWidth), mapHeight(mapHeight), cellSize(cellSize), mapData(mapData), raycaster(raycaster)
{
	float mapWidthPx = mapWidth * cellSize;
	float mapHeightPx = mapHeight * cellSize;
	scale = std::min(maxSize / mapWidthPx, maxSize / mapHeightPx);
}

void Minimap::Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle)
{
	float miniCell = cellSize * scale;
	sf::RectangleShape cell({ miniCell, miniCell });
	const float margin = 10.f;
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			int v = mapData[y * mapWidth + x];
			cell.setFillColor(v > 0 ? sf::Color(30, 60, 60) : sf::Color(180, 180, 180));
			cell.setPosition({ margin + x * miniCell, margin + y * miniCell });
			target.draw(cell);
		}
	}

	// Draw border around
	sf::RectangleShape border({ mapWidth * miniCell + 2.f, mapHeight * miniCell + 2.f });
	border.setPosition({ margin - 1.f, margin - 1.f });
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineColor(sf::Color::White);
	border.setOutlineThickness(3.f);
	target.draw(border);

	// World player position
	float worldX = playerX;
	float worldY = playerY;

	// Grid coordinates
	float cellX = worldX / cellSize;
	float cellY = worldY / cellSize;

	// Mini-map dot coordinates
	float dotX = margin + cellX * miniCell;
	float dotY = margin + cellY * miniCell;

	// Dot player on mini-map
	sf::CircleShape dot(miniCell * 0.25f);
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
		margin + (hitL.hitWorldX / cellSize) * miniCell,
		margin + (hitL.hitWorldY / cellSize) * miniCell,
	};

	sf::Vector2f endPointR{
		margin + (hitR.hitWorldX / cellSize) * miniCell,
		margin + (hitR.hitWorldY / cellSize) * miniCell,
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
			margin + (hit.hitWorldX / cellSize) * miniCell,
			margin + (hit.hitWorldY / cellSize) * miniCell,
		};

		fovFill[i + 1].position = hitPoint;
		fovFill[i + 1].color = sf::Color::Yellow;
	}

	target.draw(fovFill);
}
