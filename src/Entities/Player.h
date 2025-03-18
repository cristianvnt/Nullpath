#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
private:
	// Vars

	// Init Functions
	void InitVariables();
	void InitComponents();

public:
	Player(float x, float y, sf::Texture& texture);
	~Player();

	// Functions
};

#endif