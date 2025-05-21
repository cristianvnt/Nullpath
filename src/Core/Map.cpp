#include "Map.h"

Map::Map(int width, int height, int cellSize)
	:width(width),
	height(height),
	cellSize(cellSize),
	data(width * height),
	visited(width * height)
{
}

int Map::GetWidth() const
{
	return width;
}

int Map::GetHeight() const
{
	return height;
}

int Map::GetCellSize() const
{
	return cellSize;
}

const int* Map::GetData() const
{
	return reinterpret_cast<const int*>(data.data());
}

void Map::Clear(Cell cellType)
{
	std::fill(data.begin(), data.end(), cellType);
	ResetVisited();
}

void Map::SetCell(int x, int y, Cell c)
{
	data[y * width + x] = c;
}

Cell Map::GetCell(int x, int y) const
{
	return data[y * width + x];
}

void Map::ResetVisited()
{
	std::fill(visited.begin(), visited.end(), false);
}

bool Map::IsVisited(int x, int y) const
{
	return visited[y * width + x];
}

void Map::MarkVisited(int x, int y)
{
	visited[y * width + x] = true;
}

std::optional<std::pair<int, int>> Map::FindRandomEmpty() const
{
	auto& rng = Math::Rng();
	std::uniform_real_distribution<double> dist(0.0, 0.1);

	int count = 0;
	std::pair<int, int> chosen{ 0, 0 };

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (GetCell(x, y) == Cell::Floor)
			{
				++count;
				if (dist(rng) < 1.0 / count)
					chosen = { x, y };
			}
		}
	}
	
	if (count == 0)
		return std::nullopt;

	return chosen;
}
