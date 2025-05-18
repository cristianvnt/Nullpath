#ifndef BSPNODE_H
#define BSPNODE_H

#include <SFML/Graphics.hpp>
#include <memory>

class BSPNode
{
private:
	sf::FloatRect bounds;
	sf::Vector2f partitionLine;
	bool isVertical;
	bool isLeaf;
	std::unique_ptr<BSPNode> frontNode;
	std::unique_ptr<BSPNode> backNode;
	sf::FloatRect room;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> corridors;

	bool canSplit(int minSizePx) const;
	void choosePartition(int minSizePx, float ratio);
	void carveRoom(int minRoomSizePx, int paddingPx);
	void carveBetween(const sf::Vector2f& a, const sf::Vector2f& b);

public:
	explicit BSPNode(const sf::FloatRect& area);

	void Split(int minSizePx, float ratio = 1.25);
	void GenerateRooms(int minRoomSizePx, int paddingPx = 4);
	void CreateCorridors();
	void RenderDebug(sf::RenderWindow& window) const;

	bool IsLeaf() const { return isLeaf; }
	bool IsVertical() const { return isVertical; }
	const sf::FloatRect& Room() const { return room; }
	const auto& Corridors() const { return corridors; }

	template <typename Func>
	void ForEachLeaf(Func f) const
	{
		if (isLeaf)
		{
			f(*this);
			return;
		}

		if (frontNode)
			frontNode->ForEachLeaf(f);

		if (backNode)
			backNode->ForEachLeaf(f);
	}

};


#endif // !BSPNODE_H
