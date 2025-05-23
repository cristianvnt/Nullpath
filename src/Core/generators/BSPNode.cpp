#include "BSPNode.h"
#include "Core/Math.h"

static constexpr int MAX_DEPTH = 4;

BSPNode::BSPNode(const sf::FloatRect& area)
	: bounds(area),
	isLeaf(true),
	room({ 0.f, 0.f }, {0.f, 0.f})
{
}

bool BSPNode::canSplit(int minSizePx) const
{
	return bounds.size.x >= 2 * minSizePx && bounds.size.y >= 2 * minSizePx;
}

bool BSPNode::decideOrientation(bool canSplitH, bool canSplitV, float splitRatio, float width, float height)
{
	if (!canSplitH && !canSplitV)
		return false;

	if (!canSplitV)
		return true;
	if (!canSplitH)
		return false;

	float aspect = width / height;
	if (aspect > splitRatio)
		return false;

	if (aspect < 1.f / splitRatio)
		return true;

	std::uniform_int_distribution<int> dist(0, 1);
	return dist(Math::Rng());
}

bool BSPNode::choosePartition(const sf::FloatRect& bounds, int minSizePx, float splitRatio, bool& outHorizontal, float& outOffset)
{
	bool canSplitV = bounds.size.x >= 2 * minSizePx;
	bool canSplitH = bounds.size.y >= 2 * minSizePx;
	if (!canSplitH && !canSplitV)
		return false;

	outHorizontal = decideOrientation(canSplitH, canSplitV, splitRatio, bounds.size.x, bounds.size.y);

	float span = outHorizontal ? bounds.size.y : bounds.size.x;

	float minFrac = 1.f / (splitRatio + 1.f);
	float maxFrac = splitRatio / (splitRatio + 1.f);

	float minOff = std::max(static_cast<float>(minSizePx), span * minFrac);
	float maxOff = std::min(span - static_cast<float>(minSizePx), span * maxFrac);
	
	if (minOff > maxOff)
		return false;

	std::uniform_real_distribution<float> dist(minOff, maxOff);
	outOffset = dist(Math::Rng());
	return true;
}

void BSPNode::carveRoom(int minRoomSizePx, int paddingPx)
{
	auto& rng = Math::Rng();

	// Dynamic padding: up to paddingPx or 10% of partition size
	float padX = std::min(static_cast<float>(paddingPx), bounds.size.x * 0.1f);
	float padY = std::min(static_cast<float>(paddingPx), bounds.size.y * 0.1f);

	// Compute available space for room
	float availW = bounds.size.x - 2 * padX;
	float availH = bounds.size.y - 2 * padY;
	if (availW < minRoomSizePx || availH < minRoomSizePx) 
	{
		room = sf::FloatRect{};  // invalid room
		return;
	}

	// Room dimensions
	std::uniform_real_distribution<float> distW(static_cast<float>(minRoomSizePx), availW);
	std::uniform_real_distribution<float> distH(static_cast<float>(minRoomSizePx), availH);
	float w = distW(rng);
	float h = distH(rng);

	// Position between partition
	std::uniform_real_distribution<float> distX(0.f, availW - w);
	std::uniform_real_distribution<float> distY(0.f, availH - h);
	float x = bounds.position.x + padX + distX(rng);
	float y = bounds.position.y + padY + distY(rng);

	room = sf::FloatRect({ x, y }, { w, h });
}

void BSPNode::carveBetween(const sf::Vector2f& a, const sf::Vector2f& b)
{
	if (a.x < bounds.position.x || a.x > bounds.position.x + bounds.size.x ||
		a.y < bounds.position.y || a.y > bounds.position.y + bounds.size.y ||
		b.x < bounds.position.x || b.x > bounds.position.x + bounds.size.x ||
		b.y < bounds.position.y || b.y > bounds.position.y + bounds.size.y)
	{
		return;
	}
	corridors.emplace_back(a, b);
}

const BSPNode* BSPNode::getRoomNode() const
{
	if (isLeaf && room.size.x > 0 && room.size.y > 0)
		return this;

	const BSPNode* found = nullptr;
	if (frontNode)
		found = frontNode->getRoomNode();

	if (!found && backNode)
		found = backNode->getRoomNode();

	return found;
}

sf::Vector2f BSPNode::centerRect(const sf::FloatRect& rect)
{
	return {
		rect.position.x + rect.size.x * 0.5f,
		rect.position.y + rect.size.y * 0.5f
	};
}

void BSPNode::Split(int minSizePx, float splitRatio, int depth)
{
	if (depth >= MAX_DEPTH)
	{
		isLeaf = true;
		return;
	}

	bool horizontal;
	float offset;
	sf::FloatRect region = bounds;
	if (!canSplit(minSizePx) || !choosePartition(region, minSizePx, splitRatio, horizontal, offset))
	{
		isLeaf = true;
		return;
	}

	isLeaf = false;
	sf::FloatRect rectF = region;
	sf::FloatRect rectB = region;
	if (horizontal)
	{
		rectF.size.y = offset;
		rectB.position.y += offset;
		rectB.size.y = region.size.y - offset;
	}
	else
	{
		rectF.size.x = offset;
		rectB.position.x += offset;
		rectB.size.x = region.size.x - offset;
	}

	frontNode = std::make_unique<BSPNode>(rectF);
	backNode = std::make_unique<BSPNode>(rectB);

	frontNode->Split(minSizePx, splitRatio, depth + 1);
	backNode->Split(minSizePx, splitRatio, depth + 1);
}

void BSPNode::GenerateRooms(int minRoomSizePx, int paddingPx)
{
	if (isLeaf)
		carveRoom(minRoomSizePx, paddingPx);
	else
	{
		frontNode->GenerateRooms(minRoomSizePx, paddingPx);
		backNode->GenerateRooms(minRoomSizePx, paddingPx);
	}
}

void BSPNode::CreateCorridors()
{
	if (isLeaf || !frontNode || !backNode)
		return;

	const BSPNode* fNode = frontNode ? frontNode->getRoomNode() : nullptr;
	const BSPNode* bNode = backNode ? backNode->getRoomNode() : nullptr;

	if (fNode && bNode) 
	{
		sf::Vector2f centerF = centerRect(fNode->room);
		sf::Vector2f centerB = centerRect(bNode->room);

		carveBetween(centerF, centerB);
	}

	frontNode->CreateCorridors();
	backNode->CreateCorridors();
}

void BSPNode::RenderDebug(sf::RenderWindow& window) const
{
	sf::RectangleShape rect({ bounds.size.x, bounds.size.y });
	rect.setPosition({ bounds.position.x, bounds.position.y });
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Blue);
	rect.setOutlineThickness(2.f);
	window.draw(rect);

	if (isLeaf)
	{
		rect.setSize({ room.size.x, room.size.y });
		rect.setPosition({ room.position.x, room.position.y });
		rect.setOutlineColor(sf::Color::Magenta);
		window.draw(rect);
	}

	for (auto& c : corridors)
	{
		sf::Vertex line[] = { {c.first, sf::Color::Yellow}, {c.second, sf::Color::Yellow} };
		window.draw(line, 2, sf::PrimitiveType::Lines);
	}
}
