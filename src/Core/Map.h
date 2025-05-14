#ifndef MAP_H
#define MAP_H

#include <vector>
#include <random>
#include <array>

class Map
{
private:
	int width;
	int height;
	int tileSize;

	std::vector<int> data;
	std::vector<bool> visited;

	// RNG engine for map generation and random placements
	std::mt19937 rng;

	// DFS based map generation methods
	void CarveDFS(int x, int y);
	
public:
	Map(int width, int height, int tileSize);
	
	void GenerateMapDFS();
	std::pair<int, int> FindRandomEmpty();
	int GetWidth() const;
	int GetHeight() const;
	int GetTileSize() const;
	int GetTile(int x, int y) const;
	const int* GetData() const;
};

#endif // !MAP_H
