#include "MainMenuState.h"

void MainMenuState::InitKeybinds()
{
	std::ifstream ifs("gameStateKeybinds.ini");

	if (!ifs.is_open())
	{
		std::cerr << "Error: gameStateKeybinds.ini could not be opened.\n";
		return;
	}

	if (ifs)
	{
		std::string fnc;
		std::string keyStr;

		while (ifs >> fnc >> keyStr)
			this->keybinds[fnc] = this->supportedKeys->at(keyStr);
	}
}

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys)
	: State(window, supportedKeys)
{
	this->InitKeybinds();
	this->background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->background.setFillColor(sf::Color::Blue);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::EndState()
{
	std::cout << "MainMenuState::EndState()\n";
}

void MainMenuState::UpdateInput(const float& dt)
{
	this->CheckQuitState();
}

void MainMenuState::Update(const float& dt)
{
	this->UpdateInput(dt);
}

void MainMenuState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);
}
