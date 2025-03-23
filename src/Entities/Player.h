#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>

class Player
{
private:
	float posX, posY;
	float angle;

	float moveSpeed;
	float rotateSpeed;

	void LoadFromConfig();

public:
	Player();

	void Update(float dt);
	void UpdateMovement(float dt);
	void Render(sf::RenderTarget* target);

	float GetX() const;
	float GetY() const;
	float GetAngle() const;

};

#endif
