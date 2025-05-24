#include "Minimap.h"
#include <functional>

Minimap::Minimap(int mapWidth, int mapHeight, int cellSize, const int* mapData, Raycaster* raycaster, const std::vector<std::pair<sf::Vector2f, int>>& markers, sf::Font& f, const BSPNode* debugRoot)
	: mapWidth(mapWidth), mapHeight(mapHeight), cellSize(cellSize), mapData(mapData), raycaster(raycaster), roomMarkers(markers), font(f), debugRoot(debugRoot)
{
	float mapWidthPx = mapWidth * cellSize;
	float mapHeightPx = mapHeight * cellSize;
	scaleX = maxWidth / mapWidthPx;
	scaleY = maxHeight / mapHeightPx;
}

sf::FloatRect Minimap::WorldToMini(const sf::FloatRect& worldRect) const
{
	float cellL = worldRect.position.x / cellSize;
	float cellT = worldRect.position.y / cellSize;
	float cellW = worldRect.size.x / cellSize;
	float cellH = worldRect.size.y / cellSize;

	float miniW = cellSize * scaleX;
	float miniH = cellSize * scaleY;

	return { {margin + cellL * miniW, margin + cellT * miniH}, {cellW * miniW, cellH * miniH} };
}

void Minimap::Render(sf::RenderTarget& target, float playerX, float playerY, float playerAngle)
{
	float miniCellX = cellSize * scaleX;
	float miniCellY = cellSize * scaleY;
	sf::RectangleShape cell({ miniCellX, miniCellY });

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			int v = mapData[y * mapWidth + x];
			cell.setFillColor(v > 0 ? sf::Color(30, 60, 60) : sf::Color(180, 180, 180));
			cell.setPosition({ margin + x * miniCellX, margin + y * miniCellY });
			target.draw(cell);
		}
	}

	// Draw border around
	sf::RectangleShape border({ mapWidth * miniCellX + 2.f, mapHeight * miniCellY + 2.f });
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
	float dotX = margin + cellX * miniCellX;
	float dotY = margin + cellY * miniCellY;

	// Dot player on mini-map
	sf::CircleShape dot(miniCellX * 0.25f);
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
		margin + (hitL.hitWorldX / cellSize) * miniCellX,
		margin + (hitL.hitWorldY / cellSize) * miniCellY,
	};

	sf::Vector2f endPointR{
		margin + (hitR.hitWorldX / cellSize) * miniCellX,
		margin + (hitR.hitWorldY / cellSize) * miniCellY,
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
			margin + (hit.hitWorldX / cellSize) * miniCellX,
			margin + (hit.hitWorldY / cellSize) * miniCellY,
		};

		fovFill[i + 1].position = hitPoint;
		fovFill[i + 1].color = sf::Color::Yellow;
	}
	target.draw(fovFill);

	// Debug
	if (drawRoomIDs && debugRoot)
	{
		std::function<void(const BSPNode*)> DrawNodeDebug = [&](const BSPNode* node)
			{
				if (!node)
					return;

				// Blue partition outline
				sf::FloatRect miniBounds = WorldToMini(node->GetBounds());
				sf::RectangleShape partition({ miniBounds.size.x, miniBounds.size.y });
				partition.setPosition({ miniBounds.position.x, miniBounds.position.y });
				partition.setFillColor(sf::Color::Transparent);
				partition.setOutlineColor(sf::Color::Blue);
				partition.setOutlineThickness(2.f);
				target.draw(partition);

				if (node->IsLeaf())
				{
					// Red room carve
					sf::FloatRect miniRoom = WorldToMini(node->GetRoom());
					sf::RectangleShape roomRect({ miniRoom.size.x, miniRoom.size.y });
					roomRect.setPosition({ miniRoom.position.x, miniRoom.position.y });
					roomRect.setFillColor(sf::Color::Transparent);
					roomRect.setOutlineColor(sf::Color::Red);
					roomRect.setOutlineThickness(2.f);
					target.draw(roomRect);
				}

				DrawNodeDebug(node->Front());
				DrawNodeDebug(node->Back());
			};

		DrawNodeDebug(debugRoot);
		
		// Draw room number
		for (auto& [center, id] : roomMarkers)
		{
			float x = margin + (center.x / cellSize) * (cellSize * scaleX);
			float y = margin + (center.y / cellSize) * (cellSize * scaleY);

			sf::Text txt(font, std::to_string(id), 15);
			txt.setFillColor(sf::Color::Black);

			sf::FloatRect bounds = txt.getLocalBounds();
			txt.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
			txt.setPosition({ x, y });
			target.draw(txt);
		}
	}
}
