#include "Map.h"

Map::Map(int width, int height, int cellSize)
	:width(width),
	height(height),
	cellSize(cellSize),
	data(width * height),
	visited(width * height),
	rng(std::random_device{}())
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

std::pair<int, int> Map::FindRandomEmpty() const
{
	std::uniform_int_distribution<> distW(0, width - 1);
	std::uniform_int_distribution<> distH(0, height - 1);
	
	while (true)
	{
		int x = distW(rng);
		int y = distH(rng);
		if (GetCell(x, y) == Cell::Floor)
			return { x, y };
	}
}

std::mt19937& Map::GetRng() const
{
	return rng;
}
