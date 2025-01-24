#include "Player.h"

Player::Player(const float x, const float y, sf::Texture* texture)
{
	this->InitVariables();
	this->InitComponents();

	this->CreateSprite(texture);
	this->SetPosition(x, y);
}

Player::~Player()
{

}

void Player::InitVariables()
{
}

void Player::InitComponents()
{
	this->CreateMovementComponent(120.f);
}