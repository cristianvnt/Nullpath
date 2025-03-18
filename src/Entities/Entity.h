#ifndef ENTITY_H
#define ENTITY_H

#include "Components/MovementComponent.h"

class Entity
{
private:
	void InitVariables();

protected:
	sf::Sprite sprite;
	MovementComponent* movementComponent;

public:
	Entity(const sf::Texture& texture);
	virtual ~Entity();

	// Component functions
	void CreateMovementComponent(float maxVelocity, float acceleration, float deceleration);

	// Functions
	virtual void SetPosition(float x, float y);
	virtual void Move(float dirX, float dirY, float dt);

	virtual void Update(float dt);
	virtual void Render(sf::RenderTarget* target);
};

#endif
