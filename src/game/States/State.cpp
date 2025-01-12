#include "State.h"

State::State(sf::RenderWindow* window)
{
	this->window = window;
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		this->quit = true;
}
