#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::Sprite& sprite, float maxVelocity,
	float acceleration, float deceleration)
	: sprite(sprite), maxVelocity(maxVelocity), acceleration(acceleration), deceleration(deceleration)
{
}

MovementComponent::~MovementComponent()
{
}

const sf::Vector2f& MovementComponent::GetVelocity() const
{
	return this->velocity;
}

// Functions
void MovementComponent::Move(float dirX, float dirY, float dt)
{
	// Apply acceleration
	this->velocity.x += this->acceleration * dirX * dt;
	this->velocity.y += this->acceleration * dirY * dt;

	float magnitude = std::sqrt(this->velocity.x * this->velocity.x + this->velocity.y * this->velocity.y);
	if (magnitude > this->maxVelocity)
	{
		// Normalize velocity and scale to max velocity
		this->velocity.x = (this->velocity.x / magnitude) * this->maxVelocity;
		this->velocity.y = (this->velocity.y / magnitude) * this->maxVelocity;
	}
}

void MovementComponent::Update(float dt)
{
	float epsilon = 0.01f; // Threshold for zero velocity

	// Apply deceleration to X axis
	if (this->velocity.x > epsilon)
	{
		this->velocity.x -= this->deceleration * dt;
		if (this->velocity.x < epsilon)
			this->velocity.x = 0.f; // Stop if velocity too small
	}
	else if (this->velocity.x < -epsilon)
	{
		this->velocity.x += this->deceleration * dt;
		if (this->velocity.x > -epsilon)
			this->velocity.x = 0.f; // Stop if velocity too small
	}

	// Apply deceleration to Y axis
	if (this->velocity.y > epsilon)
	{
		this->velocity.y -= this->deceleration * dt;
		if (this->velocity.y < epsilon)
			this->velocity.y = 0.f; // Stop if velocity too small
	}
	else if (this->velocity.y < -epsilon)
	{
		this->velocity.y += this->deceleration * dt;
		if (this->velocity.y > -epsilon)
			this->velocity.y = 0.f; // Stop if velocity too small
	}

	// Move sprite based on final velocity
	this->sprite.move(this->velocity * dt);
}
