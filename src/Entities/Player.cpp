#include "Player.h"

Player::Player(float x, float y, sf::Texture& texture)
	: Entity(texture)
{
	this->InitVariables();
	this->SetPosition(x, y);
	this->InitComponents();
}

Player::~Player()
{

}

void Player::InitVariables()
{
}

void Player::InitComponents()
{
	this->CreateMovementComponent(2000.f, 500.f, 300.f);
}