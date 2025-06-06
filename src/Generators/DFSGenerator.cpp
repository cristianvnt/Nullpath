#include "DFSGenerator.h"
#include <algorithm>

void DFSGenerator::Carve(int x, int y, Map& map)
{
	// Mark current cell as empty and visited
	map.SetCell(x, y, Cell::Floor);
	map.MarkVisited(x, y);

	std::array<std::pair<int, int>, 4> dirs = {
		std::make_pair(0, -1),
		std::make_pair(1, 0),
		std::make_pair(0, 1),
		std::make_pair(-1, 0)
	};
	// Shuffle to randomize DFS order
	std::shuffle(dirs.begin(), dirs.end(), Math::Rng());

	for (auto& [dx, dy] : dirs)
	{
		int nx = x + dx * 2;
		int ny = y + dy * 2;

		if (nx >= 1 && ny >= 1 && nx < map.GetWidth() - 1 && ny < map.GetHeight() - 1 && !map.IsVisited(nx, ny))
		{
			// Remove walls between cells and recurse into next cell
			map.SetCell(x + dx, y + dy, Cell::Floor);
			Carve(nx, ny, map);
		}
	}
}

void DFSGenerator::Generate(Map& map)
{
	// Fill entire map with wall type 1
	map.Clear(Cell::Wall);
	map.ResetVisited();

	// Start at random odd coordinates
	std::uniform_int_distribution<int> distX(0, map.GetWidth() / 2 - 1);
	std::uniform_int_distribution<int> distY(0, map.GetHeight() / 2 - 1);
	int sx = distX(Math::Rng()) * 2 + 1;
	int sy = distY(Math::Rng()) * 2 + 1;

	Carve(sx, sy, map);
}
