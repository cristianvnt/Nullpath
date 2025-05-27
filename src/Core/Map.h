#ifndef MAP_H
#define MAP_H

#include "Utils/Math.h"
#include <vector>
#include <random>
#include <array>
#include <optional>

enum class Cell : int
{
	Wall = 1,
	Floor = 0
};

class Map
{
private:
	int width;
	int height;
	int cellSize;

	std::vector<Cell> data;
	std::vector<bool> visited;
	
public:
	Map(int width, int height, int cellSize);
	
	int GetWidth() const;
	int GetHeight() const;
	int GetCellSize() const;
	const int* GetData() const;

	void Clear(Cell cellType);
	void SetCell(int x, int y, Cell c);
	Cell GetCell(int x, int y) const;

	void ResetVisited();
	bool IsVisited(int x, int y) const;
	void MarkVisited(int x, int y);

	std::optional<std::pair<int, int>> FindRandomEmpty() const;
};

#endif // !MAP_H
