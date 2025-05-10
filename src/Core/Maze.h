#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <random>
#include <array>

class Maze
{
private:
	int width;
	int height;
	int tileSize;

	std::vector<int> data;
	std::vector<bool> visited;

	// RNG engine for maze generation and random placements
	std::mt19937 rng;

	// DFS based maze generation methods
	void CarveDFS(int x, int y);
	
public:
	Maze(int width, int height, int tileSize);
	
	void GenerateMazeDFS();
	std::pair<int, int> FindRandomEmpty();
	int GetWidth() const;
	int GetHeight() const;
	int GetTileSize() const;
	int GetTile(int x, int y) const;
	const int* GetData() const;
};

#endif // !MAZE_H
