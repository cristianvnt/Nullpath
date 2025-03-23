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

void Player::Update(float dt)
{
	this->UpdateMovement(dt);
	
	// DEBUG
	std::cout << "Pos: (" << this->posX << ", " << this->posY << ") Angle: " << this->angle << "\n";
}

void Player::UpdateMovement(float dt)
{
	// Move forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		this->posX += cos(this->angle) * moveSpeed * dt;
		this->posY += sin(this->angle) * moveSpeed * dt;
	}

	// Move backward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		this->posX -= cos(this->angle) * moveSpeed * dt;
		this->posY -= sin(this->angle) * moveSpeed * dt;
	}

	// Rotate left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		this->angle -= rotateSpeed * dt;
	}

	// Rotate right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		this->angle += rotateSpeed * dt;
	}

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
