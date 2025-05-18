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
	return bounds.size.x > 2 * minSizePx || bounds.size.y > 2 * minSizePx;
}

void BSPNode::choosePartition(int minSizePx, float ratio)
{
	auto& rng = Math::Rng();
	bool horizontal = std::uniform_int_distribution<int>(0, 1)(rng) == false;

	if (bounds.size.x > bounds.size.y * ratio)
		horizontal = false;
	else if (bounds.size.y > bounds.size.x * ratio)
		horizontal = true;

	float span = horizontal ? bounds.size.y : bounds.size.x;
	std::uniform_real_distribution<float> dist(minSizePx, span - minSizePx);
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
}

void BSPNode::carveRoom(int minRoomSizePx, int paddingPx)
{
	auto& rng = Math::Rng();
	float maxW = bounds.size.x - paddingPx;
	float maxH = bounds.size.y - paddingPx;
	std::uniform_real_distribution<float> distW(minRoomSizePx, maxW);
	std::uniform_real_distribution<float> distH(minRoomSizePx, maxH);
	float w = distW(rng);
	float h = distH(rng);

	std::uniform_real_distribution<float> distX(
		bounds.position.x + paddingPx / 2.f, 
		bounds.position.x + bounds.size.x - w - paddingPx / 2.f);

	std::uniform_real_distribution<float> distY(
		bounds.position.y + paddingPx / 2.f,
		bounds.position.y + bounds.size.y - h - paddingPx / 2.f);
	float x = distX(rng);
	float y = distY(rng);

	room = sf::FloatRect({ x, y }, { w, h });
}

void BSPNode::carveBetween(const sf::Vector2f& a, const sf::Vector2f& b)
{
	corridors.emplace_back(a, b);
}

void BSPNode::Split(int minSizePx, float ratio)
{
	if (!canSplit(minSizePx))
	{
		isLeaf = true;
		return;
	}
	isLeaf = false;
	choosePartition(minSizePx, ratio);

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

	frontNode->Split(minSizePx, ratio);
	backNode->Split(minSizePx, ratio);
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
	if (isLeaf)
		return;

	frontNode->CreateCorridors();
	backNode->CreateCorridors();

	sf::Vector2f c1(frontNode->room.position.x + frontNode->room.size.x / 2.f, frontNode->room.position.y + frontNode->room.size.y);
	sf::Vector2f c2(backNode->room.position.x + backNode->room.size.x / 2.f, backNode->room.position.y + backNode->room.size.y);

	carveBetween({ c1 }, { c2.x, c1.y });
	carveBetween({ c2.x, c1.y }, c2);
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
