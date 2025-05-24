#include "BSPGenerator.h"
#include "BSPNode.h"

#include <functional>
#include <algorithm>

BSPGenerator::BSPGenerator(int minRoomSizeCells, float splitRatio, int paddingCells)
	: minRoomSizeCells(minRoomSizeCells),
	splitRatio(splitRatio),
	paddingCells(paddingCells)
{
}

static inline std::pair<int, int> ToCell(const sf::Vector2f& p, int cellSize)
{
	int cellX = static_cast<int>(p.x / cellSize);
	int cellY = static_cast<int>(p.y / cellSize);
	return { cellX, cellY };
}

void BSPGenerator::Generate(Map& map)
{
	// Validate map dimensions
	if (map.GetWidth() < 2 * minRoomSizeCells || map.GetHeight() < 2 * minRoomSizeCells)
	{
		// Map is too small for BSP generation, just fill with floor
		map.Clear(Cell::Floor);
		return;
	}

	// Fill the map entirely with walls
	map.Clear(Cell::Wall);

	// Compute full map area in pixels
	float widthPx = map.GetWidth() * map.GetCellSize();
	float heightPx = map.GetHeight() * map.GetCellSize();
	sf::FloatRect fullArea({ 0.f, 0.f }, { widthPx, heightPx });

	// Build BPS tree
	rootNode = std::make_unique<BSPNode>(fullArea);
	rootNode->Split(minRoomSizeCells * map.GetCellSize(), splitRatio, 0);
	rootNode->GenerateRooms(minRoomSizeCells * map.GetCellSize(), paddingCells * map.GetCellSize());
	rootNode->CreateCorridors();

	// Carve out floor tiles for each room
	rootNode->ForEachLeaf([&](const BSPNode& node)
		{
			sf::FloatRect r = node.GetRoom();

			// Skip invalid rooms
			if (r.size.x <= 0 || r.size.y <= 0)
				return;

			auto [x0, y0] = ToCell(r.position, map.GetCellSize());
			auto [x1, y1] = ToCell({ r.position.x + r.size.x, r.position.y + r.size.y }, map.GetCellSize());

			// Ensure coordinates are within map bounds
			x0 = std::max(0, std::min(x0, map.GetWidth() - 1));
			y0 = std::max(0, std::min(y0, map.GetHeight() - 1));
			x1 = std::max(0, std::min(x1, map.GetWidth() - 1));
			y1 = std::max(0, std::min(y1, map.GetHeight() - 1));

			for (int y = y0; y <= y1; y++)
				for (int x = x0; x <= x1; x++)
					map.SetCell(x, y, Cell::Floor);
		});

	// Carve corridors between rooms using orthogonal segments
	std::function<void(const BSPNode*)> carveCorridors = [&](const BSPNode* node)
		{
			if (!node)
				return;

			for (auto& seg : node->GetCorridors())
			{
				auto& [a, b] = seg;
				auto [ax, ay] = ToCell(a, map.GetCellSize());
				auto [bx, by] = ToCell(b, map.GetCellSize());

				// Ensure coordinates are within map bounds
				ax = std::max(0, std::min(ax, map.GetWidth() - 1));
				ay = std::max(0, std::min(ay, map.GetHeight() - 1));
				bx = std::max(0, std::min(bx, map.GetWidth() - 1));
				by = std::max(0, std::min(by, map.GetHeight() - 1));

				// Horizontal segment first
				int y = ay;
				int xStart = std::min(ax, bx);
				int xEnd = std::max(ax, bx);
				for (int x = xStart; x <= xEnd; x++)
					map.SetCell(x, y, Cell::Floor);

				// then vertical segment
				int x = bx;
				int yStart = std::min(ay, by);
				int yEnd = std::max(ay, by);
				for (int yy = yStart; yy <= yEnd; yy++)
					map.SetCell(x, yy, Cell::Floor);
			}

			carveCorridors(node->Front());
			carveCorridors(node->Back());
		};
	
	// Mark each leaf with a unique ID and remember its center
	roomMarkers.clear();
	int nextID = 1;
	rootNode->ForEachLeaf([&](BSPNode& node)
		{
			sf::FloatRect r = node.GetRoom();

			if (r.size.x <= 0 || r.size.y <= 0)
				return;

			node.SetRoomID(nextID);
			roomMarkers.emplace_back(BSPNode::CenterRect(r), nextID);
			std::cout << "Room #" << node.GetRoomID() << " center=(" << BSPNode::CenterRect(r).x << "," << BSPNode::CenterRect(r).y << ")\n";
			nextID++;
		});

	// Start carving from the root
	carveCorridors(rootNode.get());
}

static void DrawNodeDebug(const BSPNode* node, sf::RenderTarget& target, const Minimap& mini)
{
	if (!node)
		return;

	// Draw node's full bounds
	sf::FloatRect bounds = node->GetBounds();
	sf::FloatRect miniBounds = mini.WorldToMini(bounds);
	sf::RectangleShape rect({ miniBounds.size.x, miniBounds.size.y });
	rect.setPosition({ miniBounds.position.x, miniBounds.position.y });
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Blue);
	rect.setOutlineThickness(2.f);
	target.draw(rect);

	// If leaf, draw the actual room inside
	if (node->IsLeaf())
	{
		sf::FloatRect room = node->GetRoom();
		sf::FloatRect miniRoom = mini.WorldToMini(room);
		sf::RectangleShape roomRect({ miniRoom.size.x, miniRoom.size.y });
		roomRect.setPosition({ miniRoom.position.x, miniRoom.position.y });
		roomRect.setFillColor(sf::Color::Transparent);
		roomRect.setOutlineColor(sf::Color::Magenta);
		roomRect.setOutlineThickness(2.f);
		target.draw(roomRect);
	}

	DrawNodeDebug(node->Front(), target, mini);
	DrawNodeDebug(node->Back(), target, mini);
}

void BSPGenerator::RenderDebug(sf::RenderTarget& target, const Minimap& mini) const
{
	if (!rootNode)
		return;

	DrawNodeDebug(rootNode.get(), target, mini);
}
