#include "BSPGenerator.h"

BSPGenerator::BSPGenerator(int minRoomSize)
	: root(nullptr),
	minRoomSize(minRoomSize)
{
}

// Node implementation
BSPGenerator::Node::Node(sf::Vector2i p, sf::Vector2i s)
{
}

bool BSPGenerator::Node::Split(int minSize)
{
	return false;
}

void BSPGenerator::Node::CreateRoom(int minSize)
{
}

void BSPGenerator::Generate(Map& map)
{
}

void BSPGenerator::SplitRecursive(std::unique_ptr<Node>& node)
{
}

void BSPGenerator::CreateRoomRecursive(const std::unique_ptr<Node>& node, Map& map)
{
}

void BSPGenerator::ConnectChildren(const std::unique_ptr<Node>& node, Map& map)
{
}

void BSPGenerator::CarveHorizontal(Map& map, int x1, int x2, int y)
{
}

void BSPGenerator::CarveVertical(Map& map, int y1, int y2, int x)
{
}
