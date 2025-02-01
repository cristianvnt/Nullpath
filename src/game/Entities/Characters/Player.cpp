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
	this->CreateMovementComponent(200.f, 50.f, 30.f);
}