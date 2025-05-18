#ifndef BPSGENERATOR_H
#define BPSGENERATOR_H

#include <SFML/Graphics.hpp>
#include "Core/Map.h"

class BSPGenerator
{
private:
	int minRoomSizeCells;
	float splitRatio;
	int paddingCells;

public:
	explicit BSPGenerator(int minRoomSizeCells = 6, float splitRatio = 1.25f, int paddingCells = 1);
	void Generate(Map& map);
};

#endif // !BPSGENERATOR_H
