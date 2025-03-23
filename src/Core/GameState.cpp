#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	this->InitPlayers();
}

GameState::~GameState()
{
	delete this->player;
}

void GameState::InitPlayers()
{
	this->player = new Player();
}

void GameState::Update(float dt)
{
	this->UpdateMousePositions();

	this->player->Update(dt);
}

void GameState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->player->Render(target);
}
