#ifndef BSPNODE_H
#define BSPNODE_H

#include <SFML/Graphics.hpp>
#include <memory>

class BSPNode
{
private:
	sf::FloatRect bounds;
	bool isLeaf;
	std::unique_ptr<BSPNode> frontNode;
	std::unique_ptr<BSPNode> backNode;
	sf::FloatRect room;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> corridors;

	bool canSplit(int minSizePx) const;

	/*
	* Determine split orientation
	* @param canH true if horizontal split possible
	* @param canV true if vertical split possible
	* @param ratio max allowed ratio between partitions
	* @return true for horizontal, false for vertical
	*/
	bool decideOrientation(bool canSplitH, bool canSplitV, float splitRatio, float width, float height);

	/*
	* Decide orientation and offset for partition respecting ratio
	* @param bounds Region to split
    * @param minSizePx Minimum size of each partition
    * @param ratio Max allowed ratio between partitions (ex: 1.25)
    * @param outHorizontal true if horizontal split, false if vertical
    * @param outOffset Distance from top/left for split line
    * @return true if a valid split can be made
	*/
	bool choosePartition(const sf::FloatRect& bounds, int minSizePx, float splitRatio, bool& outHorizontal, float& outOffset);
	void carveRoom(int minRoomSizePx, int paddingPx);
	void carveBetween(const sf::Vector2f& a, const sf::Vector2f& b);

	/*
	* Recursively find a leaf node that contains a valid room
	*/
	const BSPNode* getRoomNode() const;
	static sf::Vector2f centerRect(const sf::FloatRect& rect);

public:
	explicit BSPNode(const sf::FloatRect& area);

	/*
	* Split this node's region into two subregions using BSP.
	* @param minSizePx Minimum size in pixels for any subregion
	* @param ratio Maximum allowed ratio between subregion sizes
	* @return true if split succeeded, false otherwise
	*/
	void Split(int minSizePx, float splitRatio = 1.25f, int depth = 0);
	void GenerateRooms(int minRoomSizePx, int paddingPx = 5);

	/*
	* Generate corridors for this node (recursively calls children)
	*/
	void CreateCorridors();
	void RenderDebug(sf::RenderWindow& window) const;

	bool IsLeaf() const { return isLeaf; }
	const sf::FloatRect& Room() const { return room; }
	const auto& Corridors() const { return corridors; }

	const BSPNode* Front() const { return frontNode.get(); }
	const BSPNode* Back() const { return backNode.get(); }

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
