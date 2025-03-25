#include "Player.h"

Player::Player()
	: posX(100.f), posY(100.f), angle(0.f), moveSpeed(100.f), rotateSpeed(1.5f)
{
	this->LoadFromConfig();
}

void Player::LoadFromConfig()
{
	std::ifstream ifs("Config/playerConfig.ini");
	if (!ifs.is_open())
	{
		std::cerr << "Error: playerConfig.ini couldn't be opened.\n";
		return;
	}

	std::string key;
	while (ifs >> key)
	{
		if (key == "START_X")
			ifs >> this->posX;
		else if (key == "START_Y")
			ifs >> this->posY;
		else if (key == "MOVE_SPEED")
			ifs >> this->moveSpeed;
		else if (key == "ROTATE_SPEED")
			ifs >> this->rotateSpeed;
	}

	ifs.close();
}

void Player::Update(float dt, std::function<int(int, int)>genTileFn)
{
	this->UpdateMovement(dt, genTileFn);
	
	// DEBUG
	std::cout << "Pos: (" << this->posX << ", " << this->posY << ") Angle: " << this->angle << "\n";
}

void Player::UpdateMovement(float dt, std::function<int(int, int)>genTileFn)
{
	float nextX = this->posX;
	float nextY = this->posY;

	// Move forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		nextX += cos(this->angle) * this->moveSpeed * dt;
		nextY += sin(this->angle) * this->moveSpeed * dt;
	}

	// Move backward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		nextX -= cos(this->angle) * this->moveSpeed * dt;
		nextY -= sin(this->angle) * this->moveSpeed * dt;
	}

	// Checking collision
	int tileX = static_cast<int>(nextX) / 32;
	int tileY = static_cast<int>(nextY) / 32;

	if (genTileFn(tileX, tileY) == 0)
	{
		this->posX = nextX;
		this->posY = nextY;
	}

	// Rotate left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		this->angle -= rotateSpeed * dt;

	// Rotate right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		this->angle += rotateSpeed * dt;

}

void Player::Render(sf::RenderTarget* target)
{
	sf::CircleShape shape(5.f);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition({ this->posX, this->posY });
	target->draw(shape);
}

float Player::GetX() const
{
	return this->posX;
}

float Player::GetY() const
{
	return this->posY;
}

float Player::GetAngle() const
{
	return this->angle;
}
