#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <functional>

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

	void SetPosition(float x, float y);

	void Update(float dt, std::function<int(int, int)>genTileFn);
	void UpdateMovement(float dt, std::function<int(int, int)>genTileFn);
	void Render(sf::RenderTarget* target);

	float GetX() const;
	float GetY() const;
	float GetAngle() const;
};

#endif
