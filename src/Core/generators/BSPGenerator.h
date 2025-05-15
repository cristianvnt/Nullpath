#ifndef BPSGENERATOR_H
#define BPSGENERATOR_H

#include <SFML/Graphics.hpp>
#include "Core/Map.h"

class BSPGenerator
{
private:
	struct Node
	{
		sf::Vector2i pos, size;
		std::unique_ptr<int> left, right;
		sf::Vector2i roomPos, roomSize;

		Node(sf::Vector2i p, sf::Vector2i s);
		bool Split(int minSize);
		void CreateRoom(int minSize);
	};

	void SplitRecursive(std::unique_ptr<Node>& node);
	void CreateRoomRecursive(const std::unique_ptr<Node>& node, Map& map);
	void ConnectChildren(const std::unique_ptr<Node>& node, Map& map);
	void CarveHorizontal(Map& map, int x1, int x2, int y);
	void CarveVertical(Map& map, int y1, int y2, int x);

	int minRoomSize;
	std::unique_ptr<Node> root;

public:
	explicit BSPGenerator(int minRoomSize = 6);
	void Generate(Map& map);
};

#endif // !BPSGENERATOR_H
