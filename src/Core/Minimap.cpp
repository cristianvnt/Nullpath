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
	// Snap world coords to integer cell indices
	int x0 = static_cast<int>(std::floor(worldRect.position.x / cellSize));
	int y0 = static_cast<int>(std::floor(worldRect.position.y / cellSize));
	int x1 = static_cast<int>(std::ceil((worldRect.position.x + worldRect.size.x) / cellSize));
	int y1 = static_cast<int>(std::ceil((worldRect.position.y + worldRect.size.y) / cellSize));

	// Clamp to map bounds
	x0 = std::clamp(x0, 0, mapWidth);
	y0 = std::clamp(y0, 0, mapHeight);
	x1 = std::clamp(x1, 0, mapWidth);
	y1 = std::clamp(y1, 0, mapHeight);

	// Convert cells -> minimap pixels
	float miniCellX = cellSize * scaleX;
	float miniCellY = cellSize * scaleY;

	float mx = margin + x0 * miniCellX;
	float my = margin + y0 * miniCellY;
	float mw = (x1 - x0) * miniCellX;
	float mh = (y1 - y0) * miniCellY;

	return { {mx, my}, {mw, mh} };
}

sf::Vector2f Minimap::WorldToMiniPoint(const sf::Vector2f& worldPoint) const
{
	float miniCellX = cellSize * scaleX;
	float miniCellY = cellSize * scaleY;
	float mx = margin + (worldPoint.x / cellSize) * miniCellX;
	float my = margin + (worldPoint.y / cellSize) * miniCellY;
	return { mx, my };
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

				if (node->IsLeaf())
				{
					const sf::FloatRect& room = node->GetRoom();
					if (room.size.x > 0) {
						sf::FloatRect miniRoom = WorldToMini(room);
						sf::RectangleShape roomRect(miniRoom.size);
						roomRect.setPosition(miniRoom.position);
						roomRect.setFillColor(sf::Color::Transparent);
						roomRect.setOutlineColor(sf::Color::Red);
						roomRect.setOutlineThickness(2.f);
						target.draw(roomRect);
					}
				}

				if (node->Front() && node->Back())
				{
					sf::FloatRect frontB = node->Front()->GetBounds();
					sf::FloatRect backB = node->Back()->GetBounds();
					sf::Vector2f start, end;

					if (std::abs((frontB.position.x + frontB.size.x) - backB.position.x) < 0.1f) {
						// Vertical split
						start = { backB.position.x, backB.position.y };
						end = { backB.position.x, backB.position.y + backB.size.y };
					}
					else {
						// horizontal split
						start = { backB.position.x, backB.position.y };
						end = { backB.position.x + backB.size.x, backB.position.y };
					}

					sf::Vertex line[] = { sf::Vertex({WorldToMiniPoint(start), sf::Color::Blue}), sf::Vertex({WorldToMiniPoint(end), sf::Color::Blue}) };
					target.draw(line, 2, sf::PrimitiveType::Lines);

					DrawNodeDebug(node->Front());
					DrawNodeDebug(node->Back());
				}
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
