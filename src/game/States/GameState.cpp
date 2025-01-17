#include "GameState.h"

void GameState::InitKeybinds()
{
	std::ifstream ifs("resources/config/gameStateKeybinds.ini");

	if (!ifs.is_open())
	{
		std::cerr << "Error: gameStateKeybinds.ini could not be opened.\n";
		return;
	}

	std::string fnc;
	std::string keyStr;

	while (ifs >> fnc >> keyStr)
		this->keybinds[fnc] = this->supportedKeys->at(keyStr);
}

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys)
	: State(window, supportedKeys)
{
	this->InitKeybinds();
}

GameState::~GameState()
{
}

void GameState::EndState()
{
	std::cout << "GameState::EndState()\n";
}

void GameState::UpdateInput(const float& dt)
{
	this->CheckQuitState();
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_LEFT"]))
	{
		this->player.Move(dt, -1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_RIGHT"]))
	{
		this->player.Move(dt, 1.f, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_UP"]))
	{
		this->player.Move(dt, 0.f, -1.f);
	}
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_DOWN"]))
	{
		this->player.Move(dt, 0.f, 1.f);
	}
}

void GameState::Update(const float& dt)
{
	this->UpdateMousePositions();
	this->UpdateInput(dt);

	this->player.Update(dt);
}

void GameState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->player.Render(target);
}
