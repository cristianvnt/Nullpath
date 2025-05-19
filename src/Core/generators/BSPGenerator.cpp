#include "BSPGenerator.h"
#include "Core/generators/BSPNode.h"
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

void BSPGenerator::Generate(Map& map) const
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
	BSPNode root(fullArea);
	root.Split(minRoomSizeCells * map.GetCellSize(), splitRatio);
	root.GenerateRooms(minRoomSizeCells * map.GetCellSize(), paddingCells * map.GetCellSize());
	root.CreateCorridors();

	// Carve out floor tiles for each room
	root.ForEachLeaf([&](const BSPNode& node)
		{
			sf::FloatRect r = node.Room();

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

			for (auto& seg : node->Corridors())
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
	
	// Start carving from the root
	carveCorridors(&root);
}
