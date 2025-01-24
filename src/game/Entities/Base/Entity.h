#ifndef ENTITY_H
#define ENTITY_H

#include "Components/Movement/MovementComponent.h"

class Entity
{
private:
	void InitVariables();

protected:
	sf::Texture* texture;
	sf::Sprite* sprite;

	MovementComponent* movementComponent;

public:
	Entity();
	virtual ~Entity();

	// Component functions
	void CreateSprite(sf::Texture* texture);
	void CreateMovementComponent(const float maxVelocity);

	// Functions
	virtual void SetPosition(const float x, const float y);
	virtual void Move(const float dirX, const float dirY, const float& dt);

	virtual void Update(const float& dt);
	virtual void Render(sf::RenderTarget* target);
};

#endif
