#include "Entity.h"

Entity::Entity()
{
	this->shape.setSize(sf::Vector2f(50.f, 50.f));
	this->shape.setFillColor(sf::Color::Magenta);
	this->movementSpeed = 100.f;
}

Entity::~Entity()
{
}

void Entity::Move(const float& dt, const float dirX, const float dirY)
{
	sf::Vector2f movement(dirX * this->movementSpeed * dt, dirY * this->movementSpeed * dt);
	this->shape.move(movement.normalized());
}

void Entity::Update(const float& dt)
{
}

void Entity::Render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}
