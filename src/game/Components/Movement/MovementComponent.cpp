#include "MovementComponent.h"

MovementComponent::MovementComponent(const float maxVelocity)
{
	this->maxVelocity = maxVelocity;
}

MovementComponent::~MovementComponent()
{
}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return this->velocity;
}

// Functions
void MovementComponent::move(const float dirX, const float dirY)
{
	this->velocity.x = this->maxVelocity * dirX;
	this->velocity.y = this->maxVelocity * dirY;
}

void MovementComponent::update(const float& dt)
{
}
