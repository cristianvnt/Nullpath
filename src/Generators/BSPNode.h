#ifndef BSPNODE_H
#define BSPNODE_H

#include <SFML/Graphics.hpp>
#include <memory>

class BSPNode
{
private:
	sf::FloatRect bounds;
	bool isLeaf;
	int roomID = -1;
	std::unique_ptr<BSPNode> frontNode;
	std::unique_ptr<BSPNode> backNode;
	sf::FloatRect room;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> corridors;

	bool canSplit(int minSizePx) const;

	// Determine split orientation
	bool decideOrientation(bool canSplitH, bool canSplitV, float splitRatio, float width, float height);

	// Decide orientation and offset for partition respecting ratio
	bool choosePartition(const sf::FloatRect& bounds, int minSizePx, float splitRatio, bool& outHorizontal, float& outOffset);
	void carveRoom(int minRoomSizePx, int paddingPx);
	void carveBetween(const sf::Vector2f& a, const sf::Vector2f& b);

	// Recursively find a leaf node that contains a valid room
	const BSPNode* getRoomNode() const;

public:
	explicit BSPNode(const sf::FloatRect& area);

	// Split this node's region into two subregions using BSP.
	void Split(int minSizePx, float splitRatio = 1.25f, int depth = 0);
	void GenerateRooms(int minRoomSizePx, int paddingPx = 5);

	// Generate corridors for this node (recursively calls children)
	void CreateCorridors();

	static sf::Vector2f CenterRect(const sf::FloatRect& rect);

	bool IsLeaf() const { return isLeaf; }
	const sf::FloatRect& GetRoom() const { return room; }
	const auto& GetCorridors() const { return corridors; }
	sf::FloatRect GetBounds() const { return bounds; }

	const BSPNode* Front() const { return frontNode.get(); }
	const BSPNode* Back() const { return backNode.get(); }

	void SetRoomID(int id) { roomID = id; }
	int GetRoomID() const { return roomID; }

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

	template <typename Func>
	void ForEachLeaf(Func f)
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
