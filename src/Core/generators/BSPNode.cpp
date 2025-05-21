#include "BSPNode.h"
#include "Core/Math.h"

BSPNode::BSPNode(const sf::FloatRect& area)
	: bounds(area),
	partitionLine(0.f, 0.f),
	isVertical(false),
	isLeaf(true),
	frontNode(nullptr),
	backNode(nullptr),
	room(),
	corridors()
{
}

bool BSPNode::canSplit(int minSizePx) const
{
	const float epsilon = 2.f;
	return bounds.size.x > 2 * minSizePx - epsilon || bounds.size.y > 2 * minSizePx - epsilon;
}

bool BSPNode::choosePartition(int minSizePx, float ratio)
{
	auto& rng = Math::Rng();

	bool canV = bounds.size.x > 2 * minSizePx;
	bool canH = bounds.size.y > 2 * minSizePx;

	if (!canH && !canV) {
		return false;
	}

	bool horizontal;
	if (canH && canV)
		horizontal = std::uniform_int_distribution<int>(0, 1)(rng) == 0;
	else
		horizontal = canH;

	isVertical = !horizontal;

	float span = horizontal ? bounds.size.y : bounds.size.x;
	float maxOffset = span - minSizePx;
	
	if (maxOffset <= minSizePx)
		return false;

	std::uniform_real_distribution<float> dist(minSizePx, maxOffset);
	float offset = dist(rng);

	if (horizontal)
	{
		partitionLine.x = bounds.position.x;
		partitionLine.y = bounds.position.y + offset;
	}
	else
	{
		partitionLine.x = bounds.position.x + offset;
		partitionLine.y = bounds.position.y;
	}

	return true;
}

void BSPNode::carveRoom(int minRoomSizePx, int paddingPx)
{
	auto& rng = Math::Rng();

	// Ensure we have valid dimensions to work with
	int adjustedPadding = paddingPx / 2;
	float safeWidth = std::max(1.0f, bounds.size.x - adjustedPadding);
	float safeHeight = std::max(1.0f, bounds.size.y - adjustedPadding);
	float safeMinSize = std::min({ static_cast<float>(minRoomSizePx), safeWidth, safeHeight });

	if (safeMinSize <= 0)
	{
		room = sf::FloatRect({ 0,0 }, { 0,0 });
		return;
	}

	// Calculate max width and height for the room
	float maxW = std::max(bounds.size.x - paddingPx, safeMinSize);
	float maxH = std::max(bounds.size.y - paddingPx, safeMinSize);

	if (maxW < minRoomSizePx || maxH < minRoomSizePx) {
		room = sf::FloatRect({ 0,0 }, { 0,0 });
		return;
	}

	std::uniform_real_distribution<float> distW(minRoomSizePx, maxW);
	std::uniform_real_distribution<float> distH(minRoomSizePx, maxH);
	float w = distW(rng);
	float h = distH(rng);

	float maxX = bounds.position.x + bounds.size.x - w - paddingPx / 2.f;
	float maxY = bounds.position.y + bounds.size.y - h - paddingPx / 2.f;
	float minX = bounds.position.x + paddingPx / 2.f;
	float minY = bounds.position.y + paddingPx / 2.f;

	// Ensure valid distribution range
	if (maxX < minX)
		maxX = minX;
	if (maxY < minY)
		maxY = minY;

	float x, y;
	if (maxX > minX)
	{
		std::uniform_real_distribution<float> distX(minX, maxX);
		x = distX(rng);
	}
	else
	{
		x = minX;
	}

	if (maxY > minY)
	{
		std::uniform_real_distribution<float> distY(minY, maxY);
		y = distY(rng);
	}
	else
	{
		y = minY;
	}
	
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

void BSPNode::Split(int minSizePx, float ratio, int depth)
{
	if (depth > 5)
	{
		isLeaf = true;
		return;
	}

	if (!canSplit(minSizePx) || !choosePartition(minSizePx, ratio))
	{
		isLeaf = true;
		return;
	}

	isLeaf = false;

	if (!isVertical)
	{
		float y = partitionLine.y;
		sf::FloatRect frontRect({ bounds.position.x, bounds.position.y }, { bounds.size.x, y - bounds.position.y });
		sf::FloatRect backRect({ bounds.position.x, y }, { bounds.size.x, bounds.position.y + bounds.size.y - y });

		frontNode = std::make_unique<BSPNode>(frontRect);
		backNode = std::make_unique<BSPNode>(backRect);
	}
	else
	{
		float x = partitionLine.x;
		sf::FloatRect frontRect({ bounds.position.x, bounds.position.y }, { x - bounds.position.x, bounds.size.y });
		sf::FloatRect backRect({ x, bounds.position.y }, { bounds.position.x + bounds.size.x - x,  bounds.size.y });

		frontNode = std::make_unique<BSPNode>(frontRect);
		backNode = std::make_unique<BSPNode>(backRect);
	}

	frontNode->Split(minSizePx, ratio, depth + 1);
	backNode->Split(minSizePx, ratio, depth + 1);
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
