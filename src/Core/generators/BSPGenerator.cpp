#include "BSPGenerator.h"

BSPGenerator::BSPGenerator(int minRoomSizeCells, float splitRatio, int paddingCells)
	: minRoomSizeCells(minRoomSizeCells),
	splitRatio(splitRatio),
	paddingCells(paddingCells)
{
}

void BSPGenerator::Generate(Map& map)
{
}
