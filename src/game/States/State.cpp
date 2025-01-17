#include "State.h"

State::State(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys)
{
	this->window = window;
	this->supportedKeys = supportedKeys;
	this->quit = false;
}

State::~State()
{
}

const bool& State::GetQuit() const
{
	return this->quit;
}

void State::CheckQuitState()
{
	if (sf::Keyboard::isKeyPressed(this->keybinds["CLOSE"]))
		this->quit = true;
}

void State::UpdateMousePositions()
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}
