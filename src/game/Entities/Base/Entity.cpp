#include "Entity.h"

Entity::Entity(const sf::Texture& texture)
	: sprite(texture)
{
	this->InitVariables();
}

Entity::~Entity()
{
	delete this->movementComponent;
}

void Entity::InitVariables()
{
	this->movementComponent = nullptr;
}

// Component functions
void Entity::CreateMovementComponent(float maxVelocity, float acceleration, float deceleration)
{
	this->movementComponent = new MovementComponent(this->sprite, maxVelocity, acceleration, deceleration);
}

// Functions
void Entity::SetPosition(float x, float y)
{
	this->sprite.setPosition({ x, y });
}

void Entity::Move(float dirX, float dirY, float dt)
{
	if (!this->movementComponent)
		return;


	this->movementComponent->Move(dirX, dirY, dt); // Sets velocity
	std::cout << "Velocity: (" << this->movementComponent->GetVelocity().x << ", " << this->movementComponent->GetVelocity().y << ")\n";
}

void Entity::Update(float dt)
{
	if (!this->movementComponent)
		return;

	this->movementComponent->Update(dt);
}

void Entity::Render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
}
