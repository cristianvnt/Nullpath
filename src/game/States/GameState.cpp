#include "GameState.h"

GameState::GameState(sf::RenderWindow* window) 
	: State(window)
{
}

GameState::~GameState()
{
}

void GameState::EndState()
{
	std::cout << "GameState::EndState()\n";
}

void GameState::UpdateKeybinds(const float& dt)
{
	this->CheckQuitState();
}

void GameState::Update(const float& dt)
{
	this->UpdateKeybinds(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		std::cout << "GameState::Update()\n";
}

void GameState::Render(sf::RenderTarget* target)
{
}
