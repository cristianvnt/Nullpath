#ifndef DFSGENERATOR_H
#define DFSGENERATOR_H

#include "Core/Map.h"

class DFSGenerator
{
private:
	void Carve(int x, int y, Map& map);

public:
	DFSGenerator() = default;
	void Generate(Map& map);
};

#endif // !DFSGENERATOR_H
