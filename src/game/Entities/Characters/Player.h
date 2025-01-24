#ifndef PLAYER_H
#define PLAYER_H

#include "Entities/Base/Entity.h"

class Player : public Entity
{
private:
	// Vars

	// Init Functions
	void InitVariables();
	void InitComponents();

public:
	Player(const float x, const float y, sf::Texture* texture);
	virtual ~Player();

	// Functions
};

#endif PLAYER_H