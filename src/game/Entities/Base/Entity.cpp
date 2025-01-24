#include "Entity.h"

Entity::Entity()
{
	this->InitVariables();
}

Entity::~Entity()
{
	delete this->sprite;
}

void Entity::InitVariables()
{
	this->texture = nullptr;
	this->sprite = nullptr;
	this->movementComponent = nullptr;
}

// Component functions
void Entity::CreateSprite(sf::Texture* texture)
{
	this->texture = texture;
	this->sprite = new sf::Sprite(*this->texture);
}

void Entity::CreateMovementComponent(const float maxVelocity)
{
	this->movementComponent = new MovementComponent(maxVelocity);
}

// Functions
void Entity::SetPosition(const float x, const float y)
{
	if (!this->sprite)
		return;

	this->sprite->setPosition({ x, y });
}

void Entity::Move(const float dirX, const float dirY, const float& dt)
{
	if (!this->sprite || !this->movementComponent)
		return;

	this->movementComponent->move(dirX, dirY); // Sets velocity
	this->sprite->move(this->movementComponent->getVelocity() * dt);
}

void Entity::Update(const float& dt)
{
}

void Entity::Render(sf::RenderTarget* target)
{
	if (!this->sprite)
		return;

	target->draw(*this->sprite);
}
