#include "Maze.h"

Maze::Maze(int width, int height, int tileSize)
	:width(width),
	height(height),
	tileSize(tileSize),
	data(width * height),
	visited(width * height),
	rng(std::random_device{}())
{
}

void Maze::GenerateMazeDFS()
{
	// Fill entire grid with wall type 1
	std::fill(data.begin(), data.end(), 1);
	std::fill(visited.begin(), visited.end(), false);

	// Start at random odd coords
	int sx = (rng() % (width / 2)) * 2 + 1;
	int sy = (rng() % (height / 2)) * 2 + 1;

	CarveDFS(sx, sy);
}

void Maze::CarveDFS(int x, int y)
{
	int idx = y * width + x;
	// Mark current cell as empty and visited
	data[idx] = 0;
	visited[idx] = true;

	std::array<std::pair<int, int>, 4> dirs = {
		std::make_pair(0, -1),
		std::make_pair(1, 0),
		std::make_pair(0, 1),
		std::make_pair(-1, 0)
	};
	std::shuffle(dirs.begin(), dirs.end(), rng);

	for (auto& [dx, dy] : dirs)
	{
		int nx = x + dx * 2;
		int ny = y + dy * 2;

		if (nx >= 1 && ny >= 1 && nx < width - 1 && ny < height - 1 && !visited[ny * width + nx])
		{
			// Remove walls between cells and recurse into next cell
			data[(y + dy) * width + (x + dx)] = 0;
			CarveDFS(nx, ny);
		}
	}
}

std::pair<int, int> Maze::FindRandomEmpty()
{
	std::vector<int> empties;
	empties.reserve(width * height / 2);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (data[y * width + x] == 0)
				empties.push_back(y * width + x);

	if (empties.empty())
		return { 1, 1 };

	std::uniform_int_distribution<> dist(0, empties.size() - 1);
	int idx = empties[dist(rng)];

	return { idx % width, idx / width };
}

int Maze::GetWidth() const
{
	return width;
}

int Maze::GetHeight() const
{
	return height;
}

int Maze::GetTileSize() const
{
	return tileSize;
}

int Maze::GetTile(int x, int y) const
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return -1;
	return data[y * width + x];
}

const int* Maze::GetData() const
{
	return data.data();
}
